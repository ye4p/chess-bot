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
    fen2="r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/P1N2Q1p/1PPBBPPP/R3K2R b KQkq - 0 1"
    fen3="r3k2r/p1p1qpb1/bn1ppnp1/3PN3/1p2P3/P1N2Q1p/1PPBBPPP/R3K2R w KQkq - 0 1"
    fen4="r3k2r/p1p1qpb1/bn1ppnp1/1B1PN3/1p2P3/P1N2Q1p/1PPB1PPP/R3K2R b KQkq - 0 1"
    fen5="r3k2r/p1ppqpb1/bn2pnN1/3P4/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQkq - 0 1"
    fen6="r3k2r/p1pp1pb1/bn1qpnN1/3P4/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
    fen7="r3k2N/p1pp1pb1/bn1qpn2/3P4/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQq - 0 1"
    perft_divide(fen1, 5)
