//============================================================================
// Name        : c++.cpp
// Author      : Klemens Morgenstern
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <mw/test/parser.hpp>
#include <mw/test/data/serialization.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/archives/json.hpp>

int main(int argc, char **argv)
{
    using namespace std;
    namespace po = boost::program_options;
    namespace fs = boost::filesystem;

    po::options_description desc("Possible options");
    desc.add_options()
        ("help,H", "produce help message")
        ("include-path,I",  po::value< vector<string> >(),  "include path")
        ("input-file,i",    po::value< vector<string> >(),  "input file")
        ("template-inst,T", po::value<string>(),            "template instance file")
        ("makefile,M",      po::value<string>(),            "write makefile")
        ("backend,B",       po::value<string>(),            "test backend")
        ("data-file,D",     po::value<string>(),            "serialized test data [.json, .bin, .xml]")
        ("output-dir,O",    po::value<string>(),            "output directory")
   ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help"))
    {
        std::cout << desc << endl;
        return 1;
    }
    else if (vm.count("input-file") == 0)
    {
        cout << "No input file provided." << endl;
        cout << desc << endl;
        return 1;
    }

    mw::test::parser parser;

    if (vm.count("include-path"))
    {
        vector<fs::path> ips;

        auto vec = vm["include-path"].as<vector<string>>();
        parser.set_include_paths(ips);
    }
    if (vm.count("template-inst"))
    {
        auto tpl_write = fs::path(vm["template-inst"].as<string>());
        parser.set_tpl_inst_file(tpl_write);
    }

    fs::path input = vm["input-file"].as<string>();

    auto data = parser.parse_file(input);

    parser.write_template_inst();

    if (vm.count("data-file"))
    {
        fs::path ser = vm["data-file"].as<string>();
        fs::ofstream fstr(ser); //outstream
        auto ext = ser.extension();
        using namespace cereal;
        if (ext == ".json")
        {
            JSONOutputArchive jo(fstr);
            jo & data;
        }
        else if (ext == ".bin")
        {
            BinaryOutputArchive jo(fstr);
            jo & data;
        }
        else if (ext == ".xml")
        {
            XMLOutputArchive jo(fstr);
            jo & data;
        }
        else
        {
            cout << "invalid file suffix for data-file" << endl;
            return 1;
        }
    }

	return 0;
}

