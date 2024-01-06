#ifndef STAGE_H
#define STAGE_H

struct Stage {
    float stageId;
    float startTime;
    float endTime;
    bool didFirstStart = false;
};

#endif