// Solver.cpp
#include "Solver.hpp"

Solver::Solver()
{

}

double Solver::getWeight(int index) const
{
    return w[index];
}

void Solver::setWeight(int index, double weight)
{
    w[index] = weight;
}

void Solver::generateRandomWeights()
{
    std::random_device rd;
    std::mt19937 gen(rd()); // Random seed
    std::uniform_real_distribution<double> weight(0.0, 100.0);
    for (int i = 0; i < 9; i++)
        w[i] = weight(gen);
}

MoveResult Solver::playMove(Position pos)
{
    return negamaxMain(pos, -100000, 100000, rootDepth);
}


MoveResult Solver::negamaxMain(const Position &pos, double alpha, double beta, int depth)
{
    // Then start recursive evaluation with worst possible score
    double bestEval = -100000;
    Position bestPos;

    int deltas[] = {-6, -5, -4, -1, 1, 4, 5, 6};

    Move moves[16];
    int moveCount = 0;

    // Get a list of locations worker can move to.
    for (int worker = 0; worker < 2; worker++)
    {
        int from = pos.getCurrentWorkerPos(worker);
        for (const auto& delta : deltas) 
        {
            int to = from + delta;
            if (pos.canMove(from, to))
            {
                // temp
                if (pos.isWinningMove(to))
                    bestEval = 10000;
                
                moves[moveCount].worker = worker;
                moves[moveCount].from = from;
                moves[moveCount].to = to;
                moves[moveCount].level = pos.getLevel(to);
                moveCount++;
            }
        }
    }

    // Sort by height of new location.
    std::sort(moves, moves + moveCount, [](const Move& a, const Move& b) {
        return a.level > b.level;
    });

    // Search every new worker location.
    for (int i = 0; i < moveCount; ++i) 
    {
        int to = moves[i].to;
        int worker = moves[i].worker;
        int from = moves[i].from;
        // Search every build location from this new location.
        for (const auto& delta : deltas) 
        {
            int build = to + delta;
            if (!pos.canBuild(from, to, build)) continue;

            // Explore new position
            Position nextPos(pos);
            nextPos.play(worker, from, to, build);
            double eval = -negamaxRecur(nextPos, -beta, -alpha, depth-1);

            // Update best position if it has the best evaluation.
            if (eval > bestEval)
            {
                bestEval = eval;
                bestPos = nextPos;
            }

            // Alpha-beta pruning
            alpha = std::max(alpha, eval);
            if (alpha >= beta) 
            {
                int winner = checkForWinner(pos.turn, bestEval, pos);
                return MoveResult(bestPos, winner);
            }
        }
    }   

    int winner = checkForWinner(pos.turn, bestEval, pos);
    return MoveResult(bestPos, winner);
}

double Solver::negamaxRecur(const Position &pos, double alpha, double beta, int depth)
{   
    searchCount++;

    // Return evaluation if depth limit is reached.
    if (depth == 0)
        return evaluate(pos);

    // Then start recursive evaluation with worst possible score.
    double bestEval = -100000;

    int deltas[] = {-6, -5, -4, -1, 1, 4, 5, 6};

    // Get a list of locations worker can move to.
    Move moves[16];
    int moveCount = 0;
    for (int worker = 0; worker < 2; worker++)
    {
        int from = pos.getCurrentWorkerPos(worker);
        for (const auto& d1 : deltas) 
        {
            int to = from + d1;
            if (pos.canMove(from, to))
            {
                if (pos.isWinningMove(to))
                    return 10000;
                moves[moveCount].worker = worker;
                moves[moveCount].from = from;
                moves[moveCount].to = to;
                moves[moveCount].level = pos.getLevel(to);
                moveCount++;
            }
        }
    }

    // The player loses if there are no valid move spots.
    if (!moveCount)
        return -10000;

    // Sort by height of new location.
    std::sort(moves, moves + moveCount, [](const Move& a, const Move& b) {
        return a.level > b.level;  
    });

    // Loop over every move position.
    for (int i = 0; i < moveCount; ++i) 
    {
        int to = moves[i].to;
        int worker = moves[i].worker;
        int from = moves[i].from;
        // And loop every possible build location.
        for (const auto& d2 : deltas) 
        {
            if (depth == rootDepth && worker == 0 && from == 4 && to == 8)
                to = 8;
            int build = to + d2;
            if (!pos.canBuild(from, to, build)) continue;

            Position nextPos(pos);
            nextPos.play(worker, from, to, build);
            double eval = -negamaxRecur(nextPos, -beta, -alpha, depth-1);
            // for debug
            // std::cout << depth << ": " << from << " -> " << to << " -> " << build << " " << eval << std::endl;

            bestEval = std::max(bestEval, eval);;

            alpha = std::max(alpha, eval);
            if (alpha >= beta) 
                return bestEval;
        }
    }
    return bestEval;
}

int Solver::checkForWinner(int turn, const int eval, const Position& pos)
{
    if (eval == 10000) return turn % 2;
    else if (eval == -10000) return (turn + 1) % 2;

    return -1;
}
    

double Solver::evaluate(const Position &pos)
{
    return w[0] * currHeight(pos) + 
           w[1] * oppHeight(pos) +
           w[2] * currMobility(pos) +
           w[3] * oppMobility(pos) +
           w[4] * currCoverage(pos) +
           w[5] * oppCoverage(pos) +
           w[6] * currCentrality(pos) +
           w[7] * oppCentrality(pos) +
           w[8] * proximity(pos);
}

double Solver::currHeight(const Position &pos)
{
    double eval = 0;
    for (int i = 0; i < 2; i++)
        eval += 10 * pos.getLevel(pos.workerPos[pos.turn % 2][i]);      // Current player's workers
    return eval;
}

double Solver::oppHeight(const Position &pos)
{
    double eval = 0;
    for (int i = 0; i < 2; i++)
        eval -= 10 * pos.getLevel(pos.workerPos[(pos.turn + 1) % 2][i]); // Opponent's workers
    return eval;
}

double Solver::currMobility(const Position &pos)
{
    Bitboard clearMask = pos.board[4];
    clearMask.combine(pos.workers[pos.turn % 2]);
    clearMask.combine(pos.workers[(pos.turn + 1) % 2]);

    Bitboard moveSpots = pos.workers[pos.turn % 2]
                            .getAdjSquares()
                            .clearWithMask(clearMask);

    double eval = 0;
    for (int i = 0; i < 25; i++)
        eval += 3 * moveSpots.getBit(i);
    return eval;
}

double Solver::oppMobility(const Position &pos)
{
    Bitboard clearMask = pos.board[4];
    clearMask.combine(pos.workers[pos.turn % 2]);
    clearMask.combine(pos.workers[(pos.turn + 1) % 2]);

    Bitboard moveSpots = pos.workers[(pos.turn + 1) % 2]
                            .getAdjSquares()
                            .clearWithMask(clearMask);
    
    double eval = 0;
    for (int i = 0; i < 25; i++)
        eval -= 3 * moveSpots.getBit(i);
    return eval;
}

// Note that with how clear masks work, this will exclude being able to build
// on the spot the player is currently standing on before moving. However, 
// adding this is unnecessary since this is always true assuming they can move.
double Solver::currCoverage(const Position &pos)
{
    Bitboard clearMask = pos.board[4];
    clearMask.combine(pos.workers[pos.turn % 2]);
    clearMask.combine(pos.workers[(pos.turn + 1) % 2]);

    Bitboard buildSpots = pos.workers[pos.turn % 2]
                             .getAdjSquares()
                             .clearWithMask(clearMask)
                             .getAdjSquares()
                             .clearWithMask(clearMask);

    double eval = 0;
    for (int i = 0; i < 25; i++)
        eval += 2 * buildSpots.getBit(i);
    return eval;
}

double Solver::oppCoverage(const Position &pos)
{
    Bitboard clearMask = pos.board[4];
    clearMask.combine(pos.workers[pos.turn % 2]);
    clearMask.combine(pos.workers[(pos.turn + 1) % 2]);

    Bitboard buildSpots = pos.workers[(pos.turn + 1) % 2]
                             .getAdjSquares()
                             .clearWithMask(clearMask)
                             .getAdjSquares()
                             .clearWithMask(clearMask);
                            
    double eval = 0;
    for (int i = 0; i < 25; i++)
        eval -= 2 * buildSpots.getBit(i);
    return eval;
}

double Solver::currCentrality(const Position &pos)
{
    double eval = 0;
    for (int workerPos : pos.workerPos[pos.turn % 2])
    {
        if (workerPos == c3)
            eval += 20;
        else if ((workerPos >= 5 && workerPos < 20) &&
                 (workerPos % 5 >= 1 && workerPos % 5 < 4))
            eval += 10;
    }
    return eval;
}

double Solver::oppCentrality(const Position &pos)
{
    double eval = 0;
    for (int workerPos : pos.workerPos[(pos.turn + 1) % 2])
    {
        if (workerPos == c3)
            eval -= 20;
        else if ((workerPos >= 5 && workerPos < 20) &&
                 (workerPos % 5 >= 1 && workerPos % 5 < 4))
            eval -= 10;
    }
    return eval;
}

double Solver::proximity(const Position &pos)
{
    double eval = 0; 
    for (int oppWorkerPos : pos.workerPos[(pos.turn + 1) % 2])
    {
        int oppX = oppWorkerPos / 5;
        int oppY = oppWorkerPos % 5;
        int closestDistance = 100; // worst-case
        for (int currWorkerPos : pos.workerPos[pos.turn % 2])
        {
            int currX = currWorkerPos / 5;
            int currY = currWorkerPos % 5;
            int distance = std::max(std::abs(currX - oppX), std::abs(currY - oppY)); // Chebyshev distance
            if (distance < closestDistance)
                closestDistance = distance;
        }
        eval -= closestDistance;
    }
    return eval;
}

