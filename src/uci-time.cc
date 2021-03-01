#include "uci-time.hh"

namespace ai
{
    UCITime::UCITime()
        : movetime(DEFAULT_MOVETIME)
        , wtime(0)
        , btime(0)
        , winc(0)
        , binc(0)
    {}

    bool UCITime::set(const std::string& property, double value)
    {
        struct {
            const std::string& name;
            double& attribute;
        } properties[] = {
            { "movetime", movetime },
            { "wtime", wtime },
            { "btime", btime },
            { "winc", winc },
            { "binc", binc }
        };

        for (size_t i = 0; i < sizeof(properties) / sizeof(*properties); i++)
        {
            if (properties[i].name == property)
            {
                properties[i].attribute = value;
                return true;
            }
        }

        return false;
    }
}