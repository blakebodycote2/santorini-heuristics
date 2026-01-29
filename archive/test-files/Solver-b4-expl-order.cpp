// Solver.cpp
#include "Solver.hpp"

int Solver::minimax(const Position &pos, int alpha, int beta, int depth)
{
    int bestWorker = -1, bestMoveFrom = -1, bestMoveTo = -1, bestBuild = -1;
    searchCount++;
    // Check for Winning move first.
    int deltas[] = {-6, -5, -4, -1, 1, 4, 5, 6};
    for (int worker = 0; worker < 2; worker++)
    {
        int from = pos.getCurrentWorkerPos(worker);
        for (const auto& delta : deltas) 
        {
            int to = from + delta;
            if (pos.canMove(from, to) && pos.isWinningMove(to))
                return 10000 * depth;
        }
    }

    if (depth == 0) // Return evaluation if depth limit is reached
        return evaluate(pos);

    // Then start recursive evaluation
    int bestEval = -10000;

    for (int worker = 0; worker < 2; worker++)
    {
        int from = pos.getCurrentWorkerPos(worker);

        // Sort the locations the worker can move to by height difference.

        // Loop over every move position
        for (const auto& d1 : deltas) 
        {
            int to = from + d1;
            if (!pos.canMove(from, to)) continue;

            for (const auto& d2 : deltas) 
            {
                int build = to + d2;
                if (!pos.canBuild(from, to, build)) continue;

                Position nextPos(pos);
                nextPos.play(worker, from, to, build);
                int eval = -minimax(nextPos, -beta, -alpha, depth-1);
                // std::cout << depth << ": " << from << " -> " << to << " -> " << build << " " << eval << std::endl;

                if (eval > bestEval)
                {
                    bestEval = eval;
                    // std::cout << depth << ": " << from << " -> " << to << " -> " << build << std::endl;
                    if (depth == 4) // Store best move at root depth
                    {
                        bestWorker = worker;
                        bestMoveFrom = from;
                        bestMoveTo = to;
                        bestBuild = build;
                    }
                }

                alpha = std::max(alpha, eval);
                if (alpha >= beta) 
                {
                    return bestEval;
                }

            }
        }
    }
    // if (depth == 2) std::cout << searchCount << std::endl;
    if (depth == 4)
        std::cout << "Best move: " << bestWorker << " " << bestMoveFrom << " -> " << bestMoveTo << " -> " << bestBuild << std::endl;
    return bestEval;
    // JUST NEED TO ADD SOME WAY OF KEEPING TRACK OF BEST MOVE.
}

// recursive section
    

int Solver::evaluate(const Position &pos)
{
    int eval = 0;
    for (int i = 0; i < 2; i++)
    {
        eval += 10 * pos.getLevel(pos.workerPos[pos.turn % 2][i]);      // Current player's workers
        eval -= 10 * pos.getLevel(pos.workerPos[1 - (pos.turn % 2)][i]); // Opponent's workers
    }
    return eval;
}