import chess
import time

def perft(board, depth):
    """
    Recursively counts the total number of leaf nodes at a given depth.
    """
    if depth == 0:
        return 1

    nodes = 0
    for move in board.legal_moves:
        board.push(move)
        nodes += perft(board, depth - 1)
        board.pop()
    
    return nodes

def perft_divide(fen, depth):
    """
    Prints the perft node count for each initial move and the total.
    """
    board = chess.Board(fen)
    total_nodes = 0
    
    print(f"Perft Divide for FEN: {fen}")
    print(f"Depth: {depth}\n")
    
    start_time = time.time()
    
    # Iterate over all legal moves from the root position
    # Sorting moves ensures consistent output order (e.g., alphabetical)
    for move in sorted(board.legal_moves, key=lambda m: m.uci()):
        board.push(move)
        
        # Calculate nodes for this branch
        branch_nodes = perft(board, depth - 1)
        
        board.pop()
        
        total_nodes += branch_nodes
        print(f"{move.uci()}: {branch_nodes}")
        
    end_time = time.time()
    duration = end_time - start_time
    
    print(f"\nTotal Nodes: {total_nodes}")
    print(f"Time: {duration:.4f} seconds")
    print(f"NPS: {total_nodes / duration if duration > 0 else 0:.0f}")

# Example Usage: Standard Starting Position
if __name__ == "__main__":
    # Start Position FEN
    start_fen = chess.STARTING_FEN
    fen1="r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - "
    fen2="r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K1R1 b Qkq -"
    fen3="r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N4Q/PPPBBPPP/R3K2R b KQkq - 0 1"
    fen4="r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/1R2K2R b Kkq - 1 1"
    fen5="r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q2/PPPBBPpP/1R2K2R w Kkq - 0 2"
    fen6="r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3KR2 b Qkq - "
    fen7="r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/1R2K2R b Kkq - "
    fen8="r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q2/PPPBBPpP/1R2K2R w Kkq - "
    fen9="r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P2P/2N2Q2/PPPBBPp1/1R2K2R b Kkq - "
    perft_divide(fen9, 1)
