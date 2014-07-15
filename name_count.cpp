#include <iostream>
#include <getopt.h>

#include <osmium/io/any_input.hpp>

#include <osmium/io/any_output.hpp>

#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

void print_help() {
    std::cout << "name_count INFILE\n\n" \
              << "Outputs a name frequency table.";
}

typedef std::pair<std::string, int> namepair;
struct IntCmp {
    bool operator()(const namepair &lhs, const namepair &rhs) {
        return lhs.second > rhs.second;
    }
};

class MyCountHandler : public osmium::handler::Handler<MyCountHandler> {

    public:

    std::map<std::string, int> names;

    MyCountHandler(const std::string& fieldname) {
    }

    void node(const osmium::Node& node) {
        const char* name = node.tags().get_value_by_key("name");
        if (name) {
            std::string namestr = std::string(node.tags().get_value_by_key("name"));
            names[namestr] = names.count(namestr) == 1 ? (names[namestr] + 1) : 1;
        }
    }

    void dump() {
        std::vector<namepair> sortednames(names.begin(), names.end());
        std::partial_sort(sortednames.begin(), sortednames.begin() + 10, sortednames.end(), IntCmp());
        for (int i = 0; i < 100 && i < sortednames.size(); i++) {
            std::cout << sortednames[i].first << std::endl;
            std::cout << sortednames[i].second << std::endl;
        }
    }
};

int main(int argc, char* argv[]) {
    std::string input_filename;
    int remaining_args = argc - optind;

    if (remaining_args == 1) {
        input_filename =  argv[optind];
    } else {
        input_filename = "-";
    }

    osmium::io::Reader reader(input_filename);
    osmium::io::Header header = reader.open();

    MyCountHandler count_handler("name");

    reader.apply(count_handler);

    google::protobuf::ShutdownProtobufLibrary();

    std::ostringstream buf;

    count_handler.dump();
}
