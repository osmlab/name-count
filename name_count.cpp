#include <iostream>
#include <getopt.h>

#include <osmium/io/any_input.hpp>

#include <osmium/io/any_output.hpp>

#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

void print_help() {
    std::cout << "osmium_convert [OPTIONS] [INFILE [OUTFILE]]\n\n" \
              << "If INFILE or OUTFILE is not given stdin/stdout is assumed.\n" \
              << "File format is given as suffix in format .TYPE[.ENCODING].\n" \
              << "Use -f and -t options to force format.\n" \
              << "\nFile types:\n" \
              << "  osm     normal OSM file\n" \
              << "  osh     OSM file with history information\n" \
              << "\nFile encodings:\n" \
              << "  (none)  XML encoding\n" \
              << "  gz      XML encoding compressed with gzip\n" \
              << "  bz2     XML encoding compressed with bzip2\n" \
              << "  pbf     binary PBF encoding\n" \
              << "\nOptions:\n" \
              << "  -h, --help                This help message\n" \
              << "  -f, --from-format=FORMAT  Input format\n" \
              << "  -t, --to-format=FORMAT    Output format\n";
}

class MyCountHandler : public osmium::handler::Handler<MyCountHandler> {

    public:

    std::map<const char*, int> names;

    MyCountHandler(const std::string& fieldname) {
    }

    void node(const osmium::Node& node) {
        const char* name = node.tags().get_value_by_key("name");
        if (name) {
            names[name] = names.count(name) == 1 ? (names[name] + 1) : 1;
        }
    }

    void dump() {
        std::map<const char*, int>::iterator iter;
        for (iter = names.begin(); iter != names.end(); ++iter) {
            std::cout << iter->first << std::endl;
        }
    }
};

int main(int argc, char* argv[]) {
    std::string input_filename;
    std::string output_filename("ogr_out");
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
