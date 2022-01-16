#ifndef REPORTS_H
#define REPORTS_H

#include "factory.hpp"

class IntervalReportNotifier{
public:
    explicit IntervalReportNotifier(TimeOffset to): time_offset_(to) {};

    bool should_generate_report(Time t);

private:
    TimeOffset time_offset_;
};

class SpecificTurnsReportNotifie{
public:
    explicit SpecificTurnsReportNotifie(std::set<Time> turns): turns_(std::move(turns)) {};

    bool should_generate_report(Time t);

private:
    std::set<Time> turns_;
};

void generate_structure_report(const Factory& f,std::ostream& os);

void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t);

#endif