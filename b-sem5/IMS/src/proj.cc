// School project -- Modelling and Simulation course 
// 
// IMS 2018/19 @ FIT VUT, Brno
//
// Description:
// 
// Authors:
//  - Dušek Vladimír (xdusek27)
//  - Naňo Andrej (xnanoa00)

#include <iostream>
#include <simlib.h>
#include "debugs.h"

/*****************************************************************************/
//! time units in more readable format

    #define     SECOND      1000
    #define     MINUTE      60 * SECOND
    #define     HOUR        60 * MINUTE
    #define     DAY         24 * HOUR
    #define     YEAR        365 * DAY

//! plurals 

    #define     SECONDS     SECOND
    #define     MINUTES     MINUTE
    #define     HOURS       HOUR
    #define     DAYS        DAY
    #define     YEARS       YEAR

//! energy units

    #define     W       1
    #define     kW      1000
    #define     MW      1000 * kW


/*****************************************************************************/
//! SIMULATION SETTINGS

    const double    SIMULATION_TIME     { 1.0 * YEAR };      // total sim. time
    const long      PSEUDORANDOM_SEED   { time(NULL) };    // reset rand. gen

/*****************************************************************************/
//! STATISTICS

    // SIMLIB Stats.
    Histogram   S_AE_CAPACITY("Available energy capacity", 0, 1000, 6);
    Histogram   S_WindTurbineFailure("Wind turbine outage status", 0, 1, 2);
    TStat       S_VisitorStaysOvernight("If visitor stays overnight", 0);
    TStat       S_VisitorEnergyNeed("Energy need per visitor", 0);
    Stat        S_VisitorStayTime("Visitor stay time in hours");
    Stat        VisitorsPerDay("Visitors per day");

    // Counters
    long        CNT_VISITORS_StayNight  { 0 };
    long        CNT_VISITORS_StayDay    { 0 };
    long        CNT_VISITORS_Leave      { 0 };
    long        CNT_WT_Failures         { 0 };
    long        CNT_SP_Failures         { 0 };


/*****************************************************************************/
// CONSTANTS / INPUT VARIABLES

//! Weather, season and visitors

    #define PEAK_SEASON true
    #define LOW_SEASON  false

    // 1 year is divided into 2 season types
    const double    PEAK_SEASON_TIME    { 120.0 * DAY };    // More visitors
    const double    LOW_SEASON_TIME     { 245.0 * DAY };    // Less visitors

    // visitor arrival rate
    const double    PEAK_SEASON_RATE    { DAY / 562.0 };
    const double    LOW_SEASON_RATE     { DAY / 56.0 };

    // probability distribution of weather types and their corresponding 
    // energy generator amounts

    const double    WIND_STRENGTH_MEAN { 30 };

//! Hut

    const double    HUT_ENERGY_MEAN             { 7.5 * kW};
    const double    HUT_CONSUME_RATE            { 1 * HOUR};
    const long      HUT_GUEST_CAPACITY          { 350 };

//! Visitors 

    const double    NIGHT_VISITOR_ENERGY_MEAN   { 4.5*kW};     // kWh
    const double    DAY_VISITOR_ENERGY_MEAN     { 1*kW};

    const double    NIGHT_VISITOR_TIME          { 12.0 * HOURS };
    const double    DAY_VISITOR_TIME            { 3.0 * HOURS };


//! Wind Turbine

    const unsigned  WT_COUNT            { 3 };                  // Number of wind turbines
    const double    WT_GENERATOR_RATE   { 5.0 * MINUTES };      // Generator frequency
    const double    WT_FAILURE_RATE     { 180.0 * DAYS };       // Expected failure rate
    const double    WT_REPAIR_TIME      { 7.0 * DAYS };         // Expected repair time per SP

    const long      WT_ACCUMULATOR_CAPACITY     { 1 * MW };


//! Solar Panel

    const unsigned  SP_COUNT            { 25 };                  // Number of solar panels
    const double    SP_GENERATOR_RATE   { 5.0 * MINUTES };      // Generator frequency
    const double    SP_FAILURE_PROB     { 0.02 };               // Expected failure probability
    const double    SP_REPAIR_TIME      { 90.0 * DAYS };       // Expected repair time per SP

    const long      SP_ACCUMULATOR_CAPACITY     { 1 * MW };

//! HUT Accumulator
    
    const double    HUT_ACCUMULATOR_CAPACITY  {  5 * MW }; //  6 megawatts
    const double    HUT_ACCUMULATOR_TRANSFER_RATE  { 1.0 * MINUTE};


/*****************************************************************************/

//! Facilities / Stores / Queues

    // solar panels
    Facility SolarPanels[SP_COUNT];

    // wind turbines
    Facility WindTurbines[WT_COUNT];

    // transmitting energy to the HUT accumulator generator accs
    Facility TransmitEnergy("Energy being transmited.");

    // energy storage
    Store HutACC("HUT energy eccumulator", HUT_ACCUMULATOR_CAPACITY);
    Store SolarPanelACC("Solar panels energy eccumulator", SP_ACCUMULATOR_CAPACITY);
    Store WindTurbineACC("Wind turbine energy eccumulator", WT_ACCUMULATOR_CAPACITY);
    Store HutGuestCapacity("Maximum capacity for guests staying for the night", HUT_GUEST_CAPACITY);


/*****************************************************************************/

//! Processes

    class SeasonChange;                     // Active season time passing
    class Visitor;                          // Eisitor transaction
    class WT_Energy;                        // Energy transaction
    class WT_Failure;                       // Failure transactions
    class SP_Energy;                        // Energy transaction
    class SP_Failure;                       // Failure transactions
    class EnergyFlow;                       // Transmition of energy between generators and HUT
    class EnergyOutage;                     // Failure of the HUT accumulator/energy input
    class Restaurant;                       // Restaurant transaction
    class Staff;                            // HUT staff transaction

//! Events 

    class SwitchSeasons;                    // Transition to the next season
    class PeakSeasonVisitorGenerator;       // Generator of visitors
    class LowSeasonVisitorGenerator;        // Generator of visitors
    class WindTurbineGenerator;             // Generator of energy
    class WindTurbineFailureGenerator;      // Generator of failure
    class SolarPanelsGenerator;             // Generator of energy
    class SolarPanelsFailureGenerator;      // Generator of failure
    class EnergyUsageGenerator;             // Generator of energy use
    class OutageGenerator;                  // Generator of failure
    class FlowGenerator;                    // Generator of energy transmit
    class EnergyUsageGenerator;             // Generator of energy usage

/*****************************************************************************/
//! WIND TURBINE
/*****************************************************************************/


Stat S_WT_EnergyTotal("Energy generated by wind turbines");


class WT_Energy : public Process
{
    unsigned long energy_amount;

    void Behavior()
    {
        // choose a working wind turbine
        for (unsigned int i=0; i < SP_COUNT; i++)
        {
            if (WindTurbines[i].Busy())
            {
                continue;
            }
            else
            {
                Seize(WindTurbines[i]);
                S_WindTurbineFailure(0);

                double weather_type = Normal(WIND_STRENGTH_MEAN ,WIND_STRENGTH_MEAN * 0.2);
                
                if (weather_type < 4.5)
                {
                    energy_amount = 0;
                }
                else
                if (weather_type < 10.0)
                {
                    energy_amount = 0.1 / 12.0 * kW;
                }
                else
                if (weather_type < 20.0)
                {
                    energy_amount = 0.2 / 12.0 * kW;
                }
                else
                if (weather_type < 30.0)
                {
                    energy_amount = 0.4 / 12.0 * kW;   
                }
                else
                if (weather_type < 54.0)
                {
                    energy_amount = 0.8 / 12.0 * kW;
                }
                else
                if (weather_type < 90.0)
                {
                    energy_amount = 1.0 / 12.0 * kW;
                }
                else
                {
                    energy_amount = 0;
                }

                energy_amount *= 10;


                S_WT_EnergyTotal(energy_amount);
                

                // if there is enough storage capacity for 'energy_amount'
                if (WindTurbineACC.Free() + energy_amount < WindTurbineACC.Capacity())
                    WindTurbineACC.Leave(energy_amount);
                else
                    WindTurbineACC.Leave(WindTurbineACC.Capacity() - WindTurbineACC.Free());

                Release(WindTurbines[i]);
                return;
            }
        }
    }
};

class WT_Failure : public Process
{
    unsigned wind_turbine_idx;
    void Behavior()
    {
        Seize(WindTurbines[wind_turbine_idx], 1);
        CNT_WT_Failures++;
        S_WindTurbineFailure(1);
        Wait(Exponential(WT_REPAIR_TIME));
        Release(WindTurbines[wind_turbine_idx]);
    }
    public:
        WT_Failure(unsigned idx) : wind_turbine_idx {idx} {}
};

class WindTurbineGenerator : public Event {
    void Behavior() 
    {
        // create new energy
        for (unsigned i=0; i < WT_COUNT; i++)
            (new WT_Energy)->Activate();

        // periodically call itself
        Activate(Time+WT_GENERATOR_RATE);
    }
};

class WindTurbineFailureGenerator : public Event {
    void Behavior()
    {
        // run the fialure process
        unsigned wind_turbine_idx = static_cast<unsigned>( WT_COUNT * Random());
        (new WT_Failure(wind_turbine_idx))->Activate();
        
        // wait for another failure to happen
        Activate(Time+Normal(WT_FAILURE_RATE, WT_FAILURE_RATE * 0.1));
    }
};

/*****************************************************************************/
//! SOLAR PANELS
/*****************************************************************************/

Stat S_SP_EnergyTotal("Energy generated by solar panels");

class SP_Energy : public Process
{
    unsigned long energy_amount;

    void Behavior()
    {
        // choose a working solar panel
        for (unsigned int i=0; i < SP_COUNT; i++)
        {
            if (SolarPanels[i].Busy())
            {
                //TODO: add statistics collector
                continue;
            }
            else
            {
                Seize(SolarPanels[i]);

                int weather_type = static_cast<int>(Uniform(1.0, 12.99));
                
                switch (weather_type)
                {
                    case 1: energy_amount = (24.6 / 24.0 / 12.0) * kW; break;
                    case 2: energy_amount = (39.1 / 24.0 / 12.0) * kW; break;
                    case 3: energy_amount = (56.6 / 24.0 / 12.0) * kW; break;
                    case 4: energy_amount = (68.8 / 24.0 / 12.0) * kW; break;
                    case 5: energy_amount = (83.3 / 24.0 / 12.0) * kW; break;
                    case 6: energy_amount = (87.0 / 24.0 / 12.0) * kW; break;
                    case 7: energy_amount = (88.2 / 24.0 / 12.0) * kW; break;
                    case 8: energy_amount = (77.9 / 24.0 / 12.0) * kW; break;
                    case 9: energy_amount = (57.0 / 24.0 / 12.0) * kW; break;
                    case 10: energy_amount = (35.4 / 24.0 / 12.0) * kW; break;
                    case 11: energy_amount = (23.8 / 24.0 / 12.0) * kW; break;
                    case 12: energy_amount = (18.4 / 24.0 / 12.0) * kW; break;
                    default: energy_amount = 0;
                }

                S_SP_EnergyTotal(energy_amount);

                // if there is enough storage capacity for 'energy_amount'
                if (SolarPanelACC.Free() + energy_amount < SolarPanelACC.Capacity())
                    SolarPanelACC.Leave(energy_amount);
                else
                    SolarPanelACC.Leave(SolarPanelACC.Capacity() - SolarPanelACC.Free());

                Release(SolarPanels[i]);
                break;
            }
        }
    }
};

class SP_Failure : public Process
{
    unsigned solar_panel_idx;
    void Behavior()
    {
        // randomly choose panel
        Seize(SolarPanels[solar_panel_idx], 1);
        CNT_SP_Failures++;
        Wait(Exponential(SP_REPAIR_TIME));
        Release(SolarPanels[solar_panel_idx]);
    }
    public:
        SP_Failure(unsigned idx) : solar_panel_idx {idx} {}
};

class SolarPanelsGenerator : public Event
{
    void Behavior()
    {
        // create new energy
        for (unsigned i=0; i < SP_COUNT; i++)
            (new SP_Energy)->Activate();

        // periodically call itself
        Activate(Time+SP_GENERATOR_RATE);
    }
};

class SolarPanelsFailureGenerator : public Event
{
    void Behavior()
    {
        // run the fialure process for each panel
        for(unsigned i=0; i < SP_COUNT; i++)
        {
            if (Random() < SP_FAILURE_PROB)
            {
                (new SP_Failure(i))->Activate();
            }
        }
            
        // wait for another failure to happen
        Activate(Time + 30.0 * DAYS);
    }
};

/*****************************************************************************/
//! VISITORS 
/*****************************************************************************/

class Visitor : public Process
{
    // individual energy need per guest
    double energy_need;
    double arrival_time;

    void Behavior()
    {
        Priority = 0;

        // 1/3 doesn't go to Triglavski dom
        if (Random() < 0.33)
        {
            return; 
        }

        arrival_time = Time;

        // 4/5 stays overnight
        if (Random()<0.8)
        {
            // if there is no room, leave
            if (HutGuestCapacity.Free() < 1)
            {
                return;
            }
            else
            {
                Enter(HutGuestCapacity, 1);
            }

            //energy_need = Normal(NIGHT_VISITOR_ENERGY_MEAN, NIGHT_VISITOR_ENERGY_DISP)
            energy_need = Normal(NIGHT_VISITOR_ENERGY_MEAN, NIGHT_VISITOR_ENERGY_MEAN * 0.1);

            // requirement for energy
            if (static_cast<double>(HutACC.Free()) < energy_need)  // not enough energy in acc
            {
                // 10% leaves the Hut if there is no energy
                if (Random() < 0.1) 
                {
                    CNT_VISITORS_Leave++;
                    Leave(HutGuestCapacity, 1);
                    return;
                }
                else
                {
                    Wait(Normal(NIGHT_VISITOR_TIME, NIGHT_VISITOR_TIME * 0.1));
                    Leave(HutGuestCapacity, 1);

                    S_VisitorEnergyNeed(energy_need);
                    S_VisitorStayTime( (Time-arrival_time) / double(HOUR) );
                    CNT_VISITORS_StayNight++;
                }
            }
            else // there is enough energy in acc
            {
                Enter(HutACC, energy_need);
                // stay for the night
                Wait(Normal(NIGHT_VISITOR_TIME, NIGHT_VISITOR_TIME * 0.1));
                Leave(HutGuestCapacity, 1);

                // statistics
                S_VisitorEnergyNeed(energy_need);
                //std::cout << "Stay-time: " << int(stay_time) << "\n" << "Time-arrival_time " << int(Time - arrival_time) / HOUR<< std::endl;
                S_VisitorStayTime( (Time-arrival_time) / double(HOUR));
                CNT_VISITORS_StayNight++;
            }
        }
        else // stay just for the day
        {
            energy_need = Normal(DAY_VISITOR_ENERGY_MEAN, DAY_VISITOR_ENERGY_MEAN * 0.1);

            // requirement for energy
            if (static_cast<double>(HutACC.Free()) < energy_need)
            {
                if (Random() < 0.1) // 10% leaves the HUT if there is no energy
                {
                    CNT_VISITORS_Leave++;
                    return;
                }
                else
                {
                    // stay for few hours
                    Wait(Normal(DAY_VISITOR_TIME, DAY_VISITOR_TIME * 0.1));

                    S_VisitorEnergyNeed(0);
                    S_VisitorStayTime( (Time-arrival_time) / double(HOUR));
                    CNT_VISITORS_StayDay++;
                }
            }
            else
            {
                
                Enter(HutACC, energy_need); // requirement for energy
                Wait(Normal(DAY_VISITOR_TIME, DAY_VISITOR_TIME * 0.1)); // stay for few hours

                S_VisitorEnergyNeed(energy_need);
                S_VisitorStayTime( (Time-arrival_time) / double(HOUR));
                CNT_VISITORS_StayDay++;
            }
        }
    }
};


// generator of new visitors in the peak season
class PeakSeasonVisitorGenerator : public Event
{
    void Behavior()
    {
        (new Visitor)->Activate();
        Activate(Time+Exponential(PEAK_SEASON_RATE));
    }
};

// generator of new visitors in the low season
class LowSeasonVisitorGenerator : public Event
{
    void Behavior()
    {
        (new Visitor)->Activate();
        Activate(Time+Exponential(LOW_SEASON_RATE));
    }
};

// simple transition event called from Season Change process 
// when there is a need for season change
class SwitchSeasons : public Event
{
    public:
        bool season;
        void Behavior();
        SwitchSeasons(bool change_to_season) : season {change_to_season} {}
};


class SeasonChange : public Process
{
    public:
        bool current_season;
        void Behavior()
        {
            if (current_season == PEAK_SEASON)
            {
                PeakSeasonVisitorGenerator *gen = new PeakSeasonVisitorGenerator;

                gen->Activate();
                std::cout << "📈 PEAK SEASON " << GREEN << BOLD << "STARTED" << RESET << std::endl;
                Wait(PEAK_SEASON_TIME);
                std::cout << "📈 PEAK SEASON " << RED << BOLD << "ENDED" << RESET << std::endl;
                gen->Cancel();

                
                // change the season
                (new SwitchSeasons(!current_season))->Activate();
            }
            else if (current_season == LOW_SEASON)
            {
                LowSeasonVisitorGenerator *gen = new LowSeasonVisitorGenerator;

                gen->Activate();
                std::cout << "📉 LOW SEASON " << GREEN << BOLD << "STARTED" << RESET << std::endl;
                Wait(LOW_SEASON_TIME);
                std::cout << "📉 LOW SEASON ENDED " << RED << BOLD << "ENDED" << RESET << std::endl;
                gen->Cancel();

                // change the season
                (new SwitchSeasons(!current_season))->Activate();
            }
        }
        SeasonChange(bool starting_season) : current_season {starting_season} {}
};

// perform season change
void SwitchSeasons::Behavior()
{
    (new SeasonChange(season))->Activate();
}

/*****************************************************************************/
/*  ENERGY FLOW from generators to HUT / ACCUMULATOR PROBLEMS */
/*****************************************************************************/


class EnergyFlow : public Process
{   
    void Behavior() {
        Seize(TransmitEnergy); // PRIORITY  = 0
        
        // randomly choose from WT or SP
        if (Random() < 0.5)
        {
            long available_energy = WindTurbineACC.Free();

            if (available_energy <= 0)
            {
                Release(TransmitEnergy);
                return;
            }

            long loss = available_energy * 0.1;


            Enter(WindTurbineACC, available_energy);
            if (HutACC.Free() + available_energy - loss < HutACC.Capacity())
                HutACC.Leave(available_energy - loss);
            else
                HutACC.Leave(HutACC.Capacity() - HutACC.Free());
        }
        else 
        {
            long available_energy = SolarPanelACC.Free();

            if (available_energy <= 0)
            {
                Release(TransmitEnergy);
                return;
            }
                

            long loss = available_energy * 0.1;
        
            Enter(SolarPanelACC, SolarPanelACC.Free());
            if (HutACC.Free() + available_energy - loss < HutACC.Capacity())
                HutACC.Leave(available_energy - loss);
            else
                HutACC.Leave(HutACC.Capacity() - HutACC.Free());
        }

        Release(TransmitEnergy);
    }
};


class FlowGenerator : public Event {
    void Behavior()
    {
        (new EnergyFlow)->Activate();
        Activate(Time + Exponential(HUT_ACCUMULATOR_TRANSFER_RATE));
    }
};


/*****************************************************************************/
/*  ENERGY USAGE (restaurant.. ) */
/*****************************************************************************/

class Hut : public Process {
    void Behavior()
    {
        Priority = 1;

        double energy_need = Normal(HUT_ENERGY_MEAN, HUT_ENERGY_MEAN * 0.1);
        
        if (HutACC.Free() > energy_need)
        {
            Enter(HutACC, energy_need);
        }
        // TODO: generate statistics about outage
            
    }
};

class EnergyUsageGenerator : public Event {
    void Behavior()
    {
        (new Hut)->Activate();
        Activate(Time + HUT_CONSUME_RATE);
    }
};


class DailyStats : public Process {
    void Behavior()
    {
        long visitors_to_this_day { 0 };
        while(true)
        {
            Wait(DAY);
            VisitorsPerDay(CNT_VISITORS_StayNight+CNT_VISITORS_StayDay-visitors_to_this_day);
            visitors_to_this_day = CNT_VISITORS_StayNight+CNT_VISITORS_StayDay;
        }
    }
};


Stat S_HutCapacityUsage("Number of guests in the hut at the same time");

class MonitorGuests : public Process {
    void Behavior()
    {
        while(true)
        {
            Wait(MINUTE);
            S_HutCapacityUsage(HutGuestCapacity.Used());
            
        }
    }
};


Stat S_HutACC("Hut energy accumulator status (how much is available)");

class MonitorHutACC : public Process {
    void Behavior()
    {
        while(true)
        {
            Wait(HOUR);
            S_HutACC( HutACC.Free());
            //std::cout << "{\"x\": " << Time / static_cast<double>(DAY) << ", \"y\": " << HutACC.Free() / 1000 << ", \"c\": 0}, " << std::endl;
        }
    }
};



class SystemGenerator : public Event {
    
    void Behavior()
    {
        
        // seasons, weather, visitors
        (new SeasonChange(PEAK_SEASON))->Activate();

        // energy generators
        (new WindTurbineGenerator)->Activate(Time+MINUTE);
        (new SolarPanelsGenerator)->Activate(Time+MINUTE);

        // failures
        (new WindTurbineFailureGenerator)->Activate(Time+Exponential(WT_FAILURE_RATE));
        (new SolarPanelsFailureGenerator)->Activate(Time + 30.0 * DAY);

        // energy transfer
        (new FlowGenerator)->Activate(Time+5*MINUTES);

        // general energy consumption (restaurant, ...)
        (new EnergyUsageGenerator)->Activate();

        (new DailyStats)->Activate();
        (new MonitorGuests)->Activate();
        (new MonitorHutACC)->Activate();

    }
};


int main()
{
    // initi statistics 
    S_WT_EnergyTotal.Clear();
    S_SP_EnergyTotal.Clear();
    S_HutACC.Clear();
    S_VisitorEnergyNeed.Clear();
    S_VisitorStayTime.Clear();
    S_WindTurbineFailure.Clear();
    VisitorsPerDay.Clear();

    /*************************************************************************/
    // SETUP THE SIMULATION
    /*************************************************************************/

    RandomSeed(PSEUDORANDOM_SEED);
    Init(0,SIMULATION_TIME);

    // call the root event generator
    (new SystemGenerator)->Activate();

    /*************************************************************************/
    // START THE SIMULATION
    /*************************************************************************/
    Run();

    std::cout << BOLD << UNDERLINE << "\nNumber of days of the simulation:\t" << GREEN << (Time / double(DAY)) << RESET << std::endl;

    std::cout << "\n\n\t-------- " << UNDERLINE << BOLD << "Visitor stats" << RESET << " -------\n\n"<< std::endl;


    long stayed_total = CNT_VISITORS_StayDay + CNT_VISITORS_StayNight;
    
    std::cout << UNDERLINE << "Number of visitors TOTAL:" << RESET << "\n\n\t" << BOLD << MAGENTA << stayed_total << RESET << " visitors\n" << std::endl;
    std::cout << UNDERLINE << "Out of that:" << RESET << "\n\n\t" << BOLD << BLUE << CNT_VISITORS_StayNight << " / " << CNT_VISITORS_StayNight / double(stayed_total) * 100 << "%" << RESET << " stayed for the night 🌙" << std::endl;
    std::cout << "\n\t" << BOLD << YELLOW << CNT_VISITORS_StayDay << " / " << CNT_VISITORS_StayDay / double(stayed_total) * 100 << "%" << RESET << " stayed just during the day. ☀️\n" << std::endl; 
    std::cout << UNDERLINE << "Visitors that decided not to stay:" << RESET << "\n\n\t" << BOLD << RED << CNT_VISITORS_Leave << RESET << " immediatly left the hut. ❌ \n" << std::endl; 

    std::cout << UNDERLINE << "Visitors stayed:" << RESET << "\n\n\t" << BOLD << MAGENTA <<  S_VisitorStayTime.MeanValue() << RESET << " hours on average\n" << std::endl;
    
    S_HutCapacityUsage.Output();
    S_VisitorEnergyNeed.Output();
    VisitorsPerDay.Output();
    S_VisitorStayTime.Output();

    std::cout << "\n\n\t-------- " << UNDERLINE << BOLD << "Energy Generators" << RESET << " -------\n\n"<< std::endl;

    S_WT_EnergyTotal.Output();
    S_SP_EnergyTotal.Output();

        std::cout << "\n\n\t-------- " << UNDERLINE << BOLD << "Failures" << RESET << " -------\n\n"<< std::endl;

    std::cout << UNDERLINE << "Wind turbine failures: " << RESET << "\n\n\t" << BOLD << MAGENTA <<  CNT_WT_Failures << RESET << " failures per year\n" << std::endl;
    std::cout << UNDERLINE << "Solar panel failures: " << RESET << "\n\n\t" << BOLD << MAGENTA <<  CNT_SP_Failures << RESET << " failures per year\n" << std::endl;
    
    S_WindTurbineFailure.Output();
    
    
    std::cout << "\n\n\t-------- " << UNDERLINE << BOLD << "Hut ACC" << RESET << " -------\n\n"<< std::endl;


    S_HutACC.Output();

    return 0;
}

