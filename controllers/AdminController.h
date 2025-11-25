#ifndef ADMIN_CONTROLLER_H
#define ADMIN_CONTROLLER_H

#include <vector>
#include "../models/Pitch.h"

class AdminController {
public:
    void listPitches(const std::vector<Pitch>& pitches) const;
    void createPitch(std::vector<Pitch>& pitches);
    void updatePitch(std::vector<Pitch>& pitches);
    void deletePitch(std::vector<Pitch>& pitches);
};

#endif // ADMIN_CONTROLLER_H
