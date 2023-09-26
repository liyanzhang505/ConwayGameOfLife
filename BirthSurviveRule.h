#ifndef BIRTHSURVIVERULE_H
#define BIRTHSURVIVERULE_H
#include <iostream>

class BirthSurviveRule {
private:
    std::vector<int> birthSetting;
    std::vector<int> surviveSetting;

public:
    BirthSurviveRule(const std::vector<int>& birthSetting, const std::vector<int>& surviveSetting)
        : birthSetting(birthSetting), surviveSetting(surviveSetting) {}

    std::vector<int> getBirthSetting() const {
        return birthSetting;
    }

    std::vector<int> getSurviveSetting() const {
        return surviveSetting;
    }

    void setBirthSetting(const std::vector<int>& birthsetting) {
        this->birthSetting = birthsetting;
    }

    void setSurviveSetting(const std::vector<int>& surviveSetting) {
        this->surviveSetting = surviveSetting;
    }
};


#endif // BIRTHSURVIVERULE_H
