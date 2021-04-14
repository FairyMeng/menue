#ifndef STRUCTDEFINE_H
#define STRUCTDEFINE_H

#define POINTNUMBER 1000

//脉间数据结构体，第一次给定的数据
struct InterPulse{
    //第二次数据testData2,
    int pulse_id;
    double frequency;
    double dtoa;
    float pulse_width;
    float pulse_amp;
    long long toa;
    int midFrequencyInitialPoint;//中频数据起始点
    int midFrequencyEndPoint;//中频数据截止点

    //第一次数据millionPDW
//    int pulse_id;
//    double frequency;
//    int dtoa;
//    float pulse_width;
//    float pulse_amp;
//    long long toa;
//    int pulse_direction;
};
#endif // STRUCTDEFINE_H
