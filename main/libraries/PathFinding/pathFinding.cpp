/****************************************************************************************
 * Filename: pathFinding.cpp
 * Author: Justin Whalley
 * Description: File to contain the path finding functionality
****************************************************************************************/
#include <string.h>
#include "pathfinding.h"




void PathFinder::findObstacleRows()
{
    memset(&avoid_rows, 0, sizeof(avoid_rows));
    uint8_t row;
    uint8_t col;
    uint8_t currIndex;

    endCol = 0;
    currIndex = 0;

    for (row = 0; row < BOX_SIZE; row++)
    {
        for (col = 0; col < BOX_SIZE; col++)
        {
            if (map[row][col] == OBSTACLE)
            {
                avoid_rows[currIndex] = row;
                currIndex++;
                break;
            }

            if (map[row][col] == BORDER)
            {
                if (endCol == 0)
                {
                    endCol = col;
                }
                break;
            }
        }
    }
}

void PathFinder::findObstacleCols()
{
    memset(&avoid_cols, 0, sizeof(avoid_cols));
    uint8_t row;
    uint8_t col;
    uint8_t currIndex;

    endRow = 0;
    currIndex = 0;

    for (col = 0; col < BOX_SIZE; col++)
    {
        for (row = 0; row < BOX_SIZE; row++)
        {
            if (map[row][col] == OBSTACLE)
            {
                avoid_cols[currIndex] = col;
                currIndex++;
                break;
            }

            if (map[row][col] == BORDER)
            {
                if (endRow == 0)
                {
                    endRow = row;
                }
                break;
            }
        }
    }
}

uint8_t PathFinder::startPointsFromRange(int8_t startPoint, int8_t endPoint, uint8_t currIndex, uint8_t isHorizontal)
{
    if ((endPoint - startPoint) < VEHICLE_WIDTH)
    {
        return currIndex;
    }

    while (startPoint <= (endPoint - VEHICLE_WIDTH))
    {
        if (isHorizontal)
        {
            horizontalStartPoints[currIndex] = startPoint;
        }
        else
        {
            verticalStartPoints[currIndex] = startPoint;
        }
        startPoint = startPoint + VEHICLE_WIDTH;
        currIndex++;
    }

    if (startPoint != endPoint)
    {
        if (isHorizontal)
        {
            horizontalStartPoints[currIndex] = endPoint - VEHICLE_WIDTH + 1;
        }
        else
        {
            verticalStartPoints[currIndex] = endPoint - VEHICLE_WIDTH + 1;
        }
        currIndex++;
    }
    return currIndex;
}

void PathFinder::calculatePaths()
{
    uint8_t i;
    uint8_t y;
    uint8_t currIndex = 0;
    int8_t arr[BOX_SIZE];
    memset(&horizontalStartPoints, 0, sizeof(horizontalStartPoints));
    memset(&verticalStartPoints, 0, sizeof(verticalStartPoints));

    if ((avoid_rows[0] - 2) < VEHICLE_WIDTH)
    {
        horizontalStartPoints[0] = VEHICLE_WIDTH - 2 - (avoid_rows[0] % VEHICLE_WIDTH);
    }
    else
    {
        currIndex = startPointsFromRange(0,avoid_rows[0]-3, currIndex, 1);
    }


    for (i=1; i < BOX_SIZE; i++)
    {
        if (avoid_rows[i] != (avoid_rows[i-1] + 1))
        {
            if (avoid_rows[i] == 0 && avoid_rows[i-1] != 0)
            {
                currIndex = startPointsFromRange(avoid_rows[i-1]+2,endRow, currIndex, 1);
            }
            else if (avoid_rows[i] != 0)
            {
                currIndex = startPointsFromRange(avoid_rows[i-1]+2,avoid_rows[i]-2, currIndex, 1);
            }
        }
    }

    currIndex = 0;

    if ((avoid_cols[0] - 2) < VEHICLE_WIDTH)
    {
        verticalStartPoints[0] = VEHICLE_WIDTH - 2 - (avoid_cols[0] % VEHICLE_WIDTH);
    }
    else
    {
        currIndex = startPointsFromRange(0,avoid_cols[0] -3, currIndex, 0);
    }


    for (i=1; i < BOX_SIZE; i++)
    {
        if (avoid_cols[i] != (avoid_cols[i-1] + 1))
        {
            if (avoid_cols[i] == 0 && avoid_cols[i-1] != 0)
            {
                currIndex = startPointsFromRange(avoid_cols[i-1] +2,endCol, currIndex, 0);
            }
            else if (avoid_cols[i] != 0)
            {
                currIndex = startPointsFromRange(avoid_cols[i-1]+2,avoid_cols[i] -2, currIndex, 0);
            }
        }
    }
}