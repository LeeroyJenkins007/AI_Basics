from logic import *

AKnight = Symbol("A is a Knight")
AKnave = Symbol("A is a Knave")

BKnight = Symbol("B is a Knight")
BKnave = Symbol("B is a Knave")

CKnight = Symbol("C is a Knight")
CKnave = Symbol("C is a Knave")

# Puzzle 0
# A says "I am both a knight and a knave."
knowledge0 = And(
    # rules of the game
    Or(AKnave, AKnight),
    Not(And(AKnave, AKnight)),

    # puzzle specific
    Implication(AKnight, And(AKnave, AKnight))
)

# Puzzle 1
# A says "We are both knaves."
# B says nothing.
knowledge1 = And(
    # rules of the game
    Or(AKnave, AKnight),
    Not(And(AKnave, AKnight)),

    Or(BKnave, BKnight),
    Not(And(BKnave, BKnight)),

    # Puzzle Specific
    Biconditional(AKnight, And(AKnave, BKnave))
)

# Puzzle 2
# A says "We are the same kind."
# B says "We are of different kinds."
knowledge2 = And(
    # rules of the game
    Or(AKnight, AKnave),
    Not(And(AKnight, AKnave)),

    Or(BKnight, BKnave),
    Not(And(BKnave, BKnight)),

    # puzzle specific
    Biconditional(AKnight, Or(
        And(AKnight, BKnight),
        And(AKnave, BKnave)
    )),

    Biconditional(BKnight, Or(
        And(AKnight, BKnave),
        And(AKnave, BKnight)
    ))
)

# Puzzle 3
# A says either "I am a knight." or "I am a knave.", but you don't know which.
# B says "A said 'I am a knave'."
# B says "C is a knave."
# C says "A is a knight."
knowledge3 = And(
    # rules of the game
    Or(AKnave, AKnight),
    Not(And(AKnight, AKnave)),

    Or(BKnave, BKnight),
    Not(And(BKnave, BKnight)),

    Or(CKnight, CKnave),
    Not(And(CKnight, CKnave)),

    # puzzle specific
    Biconditional(BKnight, Biconditional(AKnight, AKnave)),
    Biconditional(CKnight, AKnight),
    Biconditional(BKnight, CKnave)
)


def main():
    symbols = [AKnight, AKnave, BKnight, BKnave, CKnight, CKnave]
    puzzles = [
        ("Puzzle 0", knowledge0),
        ("Puzzle 1", knowledge1),
        ("Puzzle 2", knowledge2),
        ("Puzzle 3", knowledge3)
    ]
    for puzzle, knowledge in puzzles:
        print(puzzle)
        if len(knowledge.conjuncts) == 0:
            print("    Not yet implemented.")
        else:
            for symbol in symbols:
                if model_check(knowledge, symbol):
                    print(f"    {symbol}")


if __name__ == "__main__":
    main()
