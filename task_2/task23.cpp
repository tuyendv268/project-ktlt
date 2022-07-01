#include "task2_library.h"

typedef struct data_statistics
{
    int id;
    int increment_frequency;
    int decrement_frequency;
}data_statistics;


vector<data_statistics> count(string file_name){
    vector<Sensor> sensors = read_csv(file_name);
    vector<data_statistics> output;

    int num_sensors = count_number_of_sensor(sensors);
    for(int id = 1; id <= num_sensors; id++){
        data_statistics temp;
        temp.id = id;
        temp.decrement_frequency = 0;
        temp.increment_frequency = 0;
        for(int i=0; i < sensors.size()-1;i++){
            if(sensors[i].id == id){
                for(int j =i+1; j<sensors.size();j++){
                    if(sensors[j].id == id){
                        if(sensors[i].values-sensors[j].values > 100){
                            temp.decrement_frequency += 1;
                        }else if(sensors[i].values-sensors[j].values < -100){
                            temp.increment_frequency += 1;
                        }
                        i = j;
                    }
                }
            }
        }
        output.push_back(temp);
    }
    return output;
}

void write_statistics_to_csv_file(vector<data_statistics> statistics, string file_name){
    FILE *file_pointer;
    file_pointer = fopen(file_name.c_str(), "w");
    fprintf(file_pointer,"id, direction, frequency\n");
    for(int i = 0; i < statistics.size();i++){
        fprintf(file_pointer,"%d, %s, %d\n", statistics[i].id, "increment", statistics[i].increment_frequency);
        fprintf(file_pointer,"%d, %s, %d\n", statistics[i].id, "decrement", statistics[i].decrement_frequency);
    }
    fclose(file_pointer);
}

int main(int argc, char ** argv){
    string file_name;
    vector<data_statistics> statistics;
    if(argc < 2){
        file_name = "speed_data.csv";
    }else{
        file_name = string(argv[1]);
    }

    statistics=count(file_name);
    write_statistics_to_csv_file(statistics, "data_statistics.csv");
    
    return 0;
}