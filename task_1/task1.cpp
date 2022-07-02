#include<bits/stdc++.h>
#include <ctime>
#include<iostream>
using namespace std;

struct UserInput{  
    int num_sensors ;
    int sampling;
    int interval;  
};

struct Time{
    int hour;
    int minute;
    int second;
};

struct Sensor{
    int id;
    Time time;
    float values;
};

bool is_number(char *);
void save_data(vector<Sensor> );
void display(vector<Sensor> );
vector<Sensor> generate_dataset(UserInput );
string convert_time_to_string(Time );
Time convert_second_to_time(int );
int convert_time_to_second(Time );
Time get_current_time();
void write_csv_file(vector<Sensor>, string);
Time add_time(Time , Time );
int parse_argument(vector<string> , const string );

// kiểm tra xem một chuỗi có phải là số hay không
bool is_number(string str){
    int size = str.length();
    for(int i = 0; i < size; i++){
        if(str[i] == '-' && i == 0){
            continue;
        }
        if(isdigit(str[i]) == false){
            return false;
        }
    }
    return true;
}

// lấy các giá trị tham số dựa vào keyword
int parse_argument(vector<string> argvs, const string keyword){
    int index = -1;
    for(int i = 0; i < argvs.size(); i++){
        if(argvs[i] == keyword){
            cout<<"find : "<<argvs[i]<<endl;
            index = i;
            if(index == argvs.size() - 1){
                return -1;
            }
            break;
        }
    }
    if(index >= argvs.size()){
        return -2;
    }
    int a = stoi(argvs[index+1]);
    cout<<"a-log; "<<(a<=0)<<endl;
    if(is_number(argvs[index+1])){
        if (stoi(argvs[index+1]) <= 0){
            return -3;
        }
        return stoi(argvs[index+1]);
    }else{
        return -1;
    }
}
//  phép cộng cho kiểu dữ liệu time
Time add_time(Time time1, Time time2){
    Time result;
    int second = time1.second + time2.second;
    result.second = second % 60;
    int minute = (int)(second/60) + time1.minute + time2.minute;
    result.minute = minute % 60;
    int hour = (int)(minute/60) + time1.hour + time2.hour;
    result.hour = hour;

    return result;
}
// lấy thời gian hiện tại
Time get_current_time(){
    time_t now = time(0);
    tm *ltm = localtime(&now);
    Time time;

    time.hour = ltm->tm_hour;
    time.minute = ltm->tm_min;
    time.second = ltm->tm_sec;
    return time;
}
// chuyển thời gian(giờ-phút-giây) sang giây
int convert_time_to_second(Time time){
    return time.hour*3600 + time.minute*60 + time.second;
}

// Phép trừ cho kiểu dữ liệu Time
Time subtract_time(Time time1, Time time2){
    int t1 = convert_time_to_second(time1);
    int t2= convert_time_to_second(time2);
    // Nếu mà t1 lớn hơn t2 thì lấy t1-t2 và ngược lại (để lấy giá trị dương)
    Time res = (t1 > t2)?convert_second_to_time(t1-t2):convert_second_to_time(t2-t1);
    return res;
}

// chuyển giây sang dạng thời gian(giờ-phút-giây)
Time convert_second_to_time(int second){
    Time time;
    time.hour = (int) second/3600;
    time.minute = (int)((second - time.hour*3600)/60);
    time.second = second - time.hour*3600 - time.minute*60;
    return time;
}

// chuyển kiểu dữ liệu time sang dạng chuỗi (string)
string convert_time_to_string(Time time){
    string hour = (time.hour >= 10)?to_string(time.hour): ("0"+to_string(time.hour));
    string minute = (time.minute >= 10)?to_string(time.minute):("0"+to_string(time.minute));
    string second = (time.second >= 10)?to_string(time.second):("0"+to_string(time.second));
    return hour + ":" + minute + ":" + second;
}
// tạo dữ liệu
vector<Sensor> generate_dataset(UserInput user_input){
    // use current time as seed for random generator
    srand(time(0));
    vector<Sensor> sensors; 
    Sensor sensor_tmp;
    Time current_time, sampling_time;
    int number_of_sampling = user_input.interval/(user_input.sampling);
    int count = 0, flag = 0;

    current_time = get_current_time();
    for(int i = 1; i <= number_of_sampling; i++){
        if(flag==1){
            break;
        }
        for(int j = 1; j <= user_input.num_sensors; j++){
            // nếu vượt quá số lần lấy mẫu sẽ kết thúc
            if(count >= number_of_sampling){
                flag = 1;
                break;
            }
            sensor_tmp.id = j;
            sensor_tmp.time = current_time;
            sensor_tmp.values = 0.2*(rand()%15000);
            sensors.push_back(sensor_tmp);
            sampling_time = convert_second_to_time(user_input.sampling);
            current_time =  add_time(current_time, sampling_time);
            count+=1;
        }
    }
    return sensors;
}

void display(vector<Sensor> sensors){
    for(int i = 0; i < sensors.size();i++){
        cout<<sensors[i].id<<","<<convert_time_to_string(sensors[i].time)<<","<<sensors[i].values<<endl;
    }
}
// Ghi dữ liệu ra file 
void write_csv_file(vector<Sensor> sensors, string file_name){
    FILE *file_pointer;
    file_pointer = fopen(file_name.c_str(), "w+");
    fprintf(file_pointer,"id, time, values\n");
    for(int i = 0; i < sensors.size();i++){
        int id = sensors[i].id;
        string time = convert_time_to_string(sensors[i].time);
        float values = sensors[i].values;
        fprintf(file_pointer,"%d, %s, %0.1f\n", id, time.c_str(), values);
    }
    fclose(file_pointer);
}

void write_to_log_file(string file_name, string str){
    FILE *file_pointer;
    file_pointer = fopen(file_name.c_str(), "a");
    fprintf(file_pointer,str.c_str());
    cout<<str<<endl;
    fclose(file_pointer);
}

void save_data(vector<Sensor> sensors){
    int starting_time = convert_time_to_second(sensors[0].time);
    string file_name = "speed_data_"+to_string(starting_time)+".csv";

    write_csv_file(sensors, file_name);
}

int main(int argc, char** argv){
    UserInput user_input;
    vector<string> argvs(argv, argv+argc);
    string log_file_name = "task1.log";

    // lấy giá trị num_sensors từ tham số mà người dùng truyền vào
    int num_sensors = parse_argument(argvs, "-n");
    // cout<<"num_sensor-log: "<<num_sensors<<endl;
    if(num_sensors == -2){
        user_input.num_sensors = 1;
    }else if(num_sensors == -1){
    // nếu có keyword -n mà không có giá trị num_sensors thì sẽ thông báo cho người dùng và kết thúc chương trình
        cout<<"you haven't enter value for -n argument !"<<endl;
        write_to_log_file(log_file_name, "error 1.1: invalid command line argument\n");
        return 0;
    }else if(num_sensors == -3){
    // nếu giá trị nhỏ hơn 0 thì sẽ báo lỗi
        write_to_log_file(log_file_name, "error 1.2: invalid number of sensors\n");
        return 0;
    }else{
    // nếu không có keyword -n và num_sensors thì lấy giá trị mặc định là 1
        user_input.num_sensors = num_sensors;
    }
    cout<<user_input.num_sensors<<endl;

    // lấy giá trị sampling từ tham số mà người dùng truyền vào
    int sampling = parse_argument(argvs, "-st");
    if(sampling == -2){
    // nếu không có keyword -st và sampling thì lấy giá trị mặc định là 10
        user_input.sampling = 10;
    }else if(sampling == -1){
    // nếu có keyword -st mà không có giá trị sampling thì sẽ thông báo cho người dùng và kết thúc chương trình
        cout<<"you haven't enter value for -st argument !"<<endl;
        write_to_log_file(log_file_name, "error 1.1: invalid command line argument\n");
        return 0;
    }else if(sampling == -3){
    // nếu giá trị nhỏ hơn 0 thì sẽ báo lỗi
        write_to_log_file(log_file_name, "error 1.3: invalid number of sampling time\n");
        return 0;
    }else{
        user_input.sampling = sampling;
    }
    cout<<user_input.sampling<<endl;
    
    // lấy giá trị interval từ tham số mà người dùng truyền vào
    int interval = parse_argument(argvs, "-si");
    if(interval == -2){
    // nếu không có keyword -si và interval thì lấy giá trị mặc định là 3600
        user_input.interval = 3600;
    }else if(interval == -1){
    // nếu có keyword -si mà không có giá trị interval thì sẽ thông báo cho người dùng và kết thúc chương trình
        cout<<"you haven't enter value for -si argument !"<<endl;
        write_to_log_file(log_file_name, "error 1.1: invalid command line argument\n");
        return 0;
    }else if(interval == -3 ||  interval<sampling){
    // nếu giá trị nhỏ hơn 0 thì sẽ báo lỗi
        write_to_log_file(log_file_name, "error 1.4: invalid duration\n");
        return 0;
    }else{
        user_input.interval = interval;
    }
    cout<<user_input.interval<<endl;
    vector<Sensor> sensors = generate_dataset(user_input);
    save_data(sensors);
    // display(sensors);
    return 0;
}