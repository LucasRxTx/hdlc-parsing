import sys
from typing import NamedTuple
import typing
import yahdlc

raw_frames_t = typing.NewType("raw_frames_t", list[bytes])
frames_t = typing.NewType("frames_t", list[bytes])
data_frames_t = typing.NewType("data_frames_t", list["DataFrame"])
moves_t = typing.NewType("moves_t", list[int])


class DataFrame(NamedTuple):
    data: bytes
    frame_type: int
    sequence_number: int

    def is_data(self) -> bool:
        return self.frame_type == yahdlc.FRAME_DATA

    def is_ack(self) -> bool:
        return self.frame_type == yahdlc.FRAME_ACK

    def is_nack(self) -> bool:
        return self.frame_type == yahdlc.FRAME_NACK

    def get_data_as_int(self) -> bool:
        return int.from_bytes(self.data, sys.byteorder)


class DataFrameParser:
    __slots__ = ("__data",)

    def __init__(self, data: bytes):
        self.__data = data

    @classmethod
    def from_file(cls, file_path: str):
        with open("transmission.bin", "rb") as f:
            data = f.read()

        return cls(data)

    def __split_data_on_flag_sequence(self, data: bytes) -> raw_frames_t:
        return raw_frames_t(data.split(b"\x7e"))

    def __filter_empty_frames(self, frames: raw_frames_t) -> raw_frames_t:
        return raw_frames_t([frame for frame in frames if frame])

    def __add_delimiters(self, frames: raw_frames_t) -> frames_t:
        return frames_t([b"\x7e" + frame + b"\x7e" for frame in frames])

    def __get_data_from_frames(self, frames: frames_t) -> data_frames_t:
        return data_frames_t([DataFrame(*yahdlc.get_data(frame)) for frame in frames])

    def __filter_data_frames(self, frames: data_frames_t) -> data_frames_t:
        return data_frames_t([frame for frame in frames if frame.is_data()])

    def __get_moves_from_data(self, data: data_frames_t) -> moves_t:
        return moves_t([d.get_data_as_int() for d in data])

    def __filter_moves_that_repeat_three_times_in_a_row(self, moves: moves_t) -> moves_t:
        out_moves = []

        # buffer all moves into same_moves, and move them into out_moves when a
        # different move is found, or throw them away if they exceed 3 times in a row.
        moves_count = len(moves)
        for i, move in enumerate(moves):
            if i < moves_count - 2 and move == moves[i + 1] and move == moves[i + 2]:
                continue

            out_moves.append(move)



        return out_moves

    def parse(self) -> moves_t:
        """Parses the data and return a list of moves.

        Returns:
            list[int]: A list of moves.
        """
        if not self.__data:
            return moves_t([])

        frames = self.__split_data_on_flag_sequence(self.__data)
        frames = self.__filter_empty_frames(frames)
        frames = self.__add_delimiters(frames)

        data_frames = self.__get_data_from_frames(frames)
        data_frames = self.__filter_data_frames(data_frames)

        moves = self.__get_moves_from_data(data_frames)
        moves = moves_t(self.__filter_moves_that_repeat_three_times_in_a_row(moves))

        return moves
