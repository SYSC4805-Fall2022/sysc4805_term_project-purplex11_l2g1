#ifndef PATH_FINDING_H
#define PATH_FINDING_H

#include <stdint.h>
#define LEN(X)                      (sizeof(X)/sizeof(X[0]))

#define VEHICLE_WIDTH   8
#define BOX_SIZE        150  
#define MAX_PASSES      20

#define UNDEFINED       0
#define BORDER          1
#define EMPTY           2
#define OBSTACLE        3
 
class PathFinder
{
public:
    int8_t map[BOX_SIZE][BOX_SIZE];
    int8_t avoid_rows[BOX_SIZE];
    int8_t avoid_cols[BOX_SIZE];
    int8_t horizontalStartPoints[MAX_PASSES];
    int8_t verticalStartPoints[MAX_PASSES];
    int8_t endCol;
    int8_t endRow;

    void findObstacleRows();
    void findObstacleCols();
    uint8_t startPointsFromRange(int8_t startPoint, int8_t endPoint, uint8_t currIndex, uint8_t isHorizontal);

    void calculatePaths();
};

#endif