#include "task2_library.h"
#include <time.h>
#include <chrono>
#include <iostream>
#include <sys/time.h>
#include <ctime>

int parse_argument(vector<string> argvs, const string keyword){
    int index = -1;
    for(int i = 0; i < argvs.size(); i++){
        if(argvs[i] == keyword){
            cout<<"find : "<<argvs[i]<<endl;
            index = i;
            return 1;
        }
    }
    return 0;
}

vector<Sensor> &id_sort(vector<Sensor> &sensors){
    int n = sensors.size(), j;
    Sensor key;
    for (int i = 1; i < n; i++)
    {
        key = sensors[i];
        j = i - 1;
 
        while (j >= 0 && sensors[j].id > key.id)
        {
            sensors[j + 1] = sensors[j];
            j = j - 1;
        }
        sensors[j + 1] = key;
    }
    return sensors;
}

vector<Sensor> &speed_sort(vector<Sensor> &sensors){
    int n = sensors.size(), j;
    Sensor key;
    for(int k = 1; k <= 3; k++){
        for (int i = 1; i < n; i++){
            key = sensors[i];
            if(key.id != k){
                continue;
            }
            j = i - 1;
    
            while (j >= 0 && sensors[j].values > key.values && sensors[j].id==k){
                sensors[j + 1] = sensors[j];
                j = j - 1;
            }
            sensors[j + 1] = key;
        }
    }
    return sensors;
}

int main(int argc, char** argv){
    vector<string> argvs(argv, argv+argc);
    int sort_options = parse_argument(argvs, "-n");

    vector<Sensor> sensors = read_csv(argvs[1]);
    struct timeval time_now{};
    gettimeofday(&time_now, NULL);
    time_t start = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);
    sensors = id_sort(sensors);
    sensors = speed_sort(sensors);
    gettimeofday(&time_now, NULL);
    time_t end = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);
    // print_sensors(sensors);
    // cout<<(end-start)/1000.0<<endl;

    string file_name = "sorted_data.csv";
    FILE *file_pointer;
    file_pointer = fopen(file_name.c_str(), "w");
    fprintf(file_pointer,"sorting duration: %f\n", (end-start)/1000.0);
    fclose(file_pointer);
    write_sensors_to_csv_file(sensors, file_name);
    return 0;
}