#include "task2_library.h"

void save_outlier_data_point(string file_name){
    vector<Sensor> outlier_sensors = get_invalid_sensors(file_name, 900, 1600);
    FILE *file_pointer;
    file_pointer = fopen("outlier_data.csv", "w");
    fprintf(file_pointer,"number of outliers: %d\n", (int)outlier_sensors.size());
    fclose(file_pointer);
    write_sensors_to_csv_file(outlier_sensors, "outlier_data.csv");
}

int main(int argc, char ** argv){
    string file_name;
    if(argc < 2){
        file_name = "speed_data.csv";
    }else{
        file_name = string(argv[1]);
    }
    // cout<<file_name<<endl;
    save_outlier_data_point(file_name);
    return 0;
}