#include "simulation.hpp"

/*!
 Funkcja sluzaca do przeprowadzania symulacji sieci.
 @param[in] f
 @param[in]  d //TODO: wydaje mi sie ze to jest czas symulacji
 @param[out]  rf funkcja sluzaca do generowania raportow
 */
void simulate(Factory& f, TimeOffset d, const std::function<void (Factory&, Time)>& rf){

    if(!f.is_consistent()){
        throw std::logic_error("fabryka jest nie spojna");
    }

    for(Time simulation_time = 1; simulation_time <= d; simulation_time++){

        //1. delivery time
        f.do_deliveres(simulation_time);

        //2. package passing
        f.do_package_passing();

        //3. work
        f.do_work(simulation_time);

        //4. raport
        rf(f, simulation_time);
    }
}
