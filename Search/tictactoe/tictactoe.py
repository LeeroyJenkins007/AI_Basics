"""
Tic Tac Toe Player
"""

import math
import copy

X = "X"
O = "O"
EMPTY = None


def initial_state():
    """
    Returns starting state of the board.
    """
    return [[EMPTY, EMPTY, EMPTY],
            [EMPTY, EMPTY, EMPTY],
            [EMPTY, EMPTY, EMPTY]]


def player(board):
    """
    Returns player who has the next turn on a board.
    """
    x_total = sum(row.count(X) for row in board)
    o_total = sum(row.count(O) for row in board)

    if x_total <= o_total:
        return X
    else:
        return O


def actions(board):
    """
    Returns set of all possible actions (i, j) available on the board.
    """
    action_set = set()

    for i, row in enumerate(board):
        for j, val in enumerate(row):
            if val is None:
                action_set.add((i, j))

    return action_set


def result(board, action):
    """
    Returns the board that results from making move (i, j) on the board.
    """

    # there is already an X or an O then
    if board[action[0]][action[1]] is not None:
        raise NameError('Not a valid action for this board state')
    

    result_board = copy.deepcopy(board)

    result_board[action[0]][action[1]] = player(board)
    return result_board


def winner(board):
    """
    Returns the winner of the game, if there is one.
    """

    check_spaces = []
    blength = len(board)
    winner = None
    
    # adds rows to the checklist
    check_spaces.extend(board)

    # adds columns to the checklist
    columns = [[row[col] for row in board] for col in range(blength)]
    check_spaces.extend(columns)

    # adds diagnals
    check_spaces.append([board[i][i] for i in range(blength)])
    check_spaces.append([board[i][blength-1-i] for i in range(blength)])


    for test in check_spaces:
        if test.count(X) == 3:
            winner = X
            break
        elif test.count(O) == 3:
            winner = O
            break

    return winner




def terminal(board):
    """
    Returns True if game is over, False otherwise.
    """
    if winner(board) != None or sum(row.count(None) for row in board) == 0:
        return True
    else:
        return False


def utility(board):
    """
    Returns 1 if X has won the game, -1 if O has won, 0 otherwise.
    """
    theWinner = winner(board)

    if theWinner == X:
        val = 1
    elif theWinner == O:
        val = -1
    else:
        val = 0

    return val


def minimax(board):
    """
    Returns the optimal action for the current player on the board.
    """
    if terminal(board):
        return None
    else:
        if player(board) == X:
            previous_score = float('-inf')
            for action in actions(board):
                cur_score = min_value(result(board, action))
                if  cur_score >= previous_score:
                    next_move = action
                    previous_score = cur_score
        else:
            previous_score = float('+inf')
            for action in actions(board):
                cur_score = max_value(result(board, action))
                if cur_score <= previous_score:
                    next_move = action
                    previous_score = cur_score
    return next_move


def max_value(board):
    v = float('-inf')
    if terminal(board):
        return utility(board)
    else:
        for action in actions(board):
            v = max(v, min_value(result(board, action)))
        
        return v

def min_value(board):
    v = float('+inf')
    if terminal(board):
        return utility(board)
    else:
        for action in actions(board):
            v = min(v, max_value(result(board, action)))

        return v