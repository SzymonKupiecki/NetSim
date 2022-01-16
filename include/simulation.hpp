#include "factory.hpp"


void simulate(Factory& f, TimeOffset d, const std::function<void (Factory&, Time)>& rf);


class IntervalReportNotifier{
public:
    explicit IntervalReportNotifier(TimeOffset to): time_offset_(to) {};

    bool should_generate_report(Time t);

private:
    TimeOffset time_offset_;
};

class SpecificTurnsReportNotifie{
public:
    explicit SpecificTurnsReportNotifie(std::set<Time> turns): turns_(turns) {};

    bool should_generate_report(Time t);

private:
    std::set<Time> turns_;
};