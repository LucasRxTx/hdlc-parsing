from player import Player
from data_frame import DataFrameParser

if __name__ == "__main__":
    parser = DataFrameParser.from_file("transmission.bin")
    moves = parser.parse()
    player = Player(0, 4)

    for move in moves:
        player.move(move)

    print(f"Player ended at ({player.x}, {player.y})")
