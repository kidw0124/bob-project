#include<bits/stdc++.h>
using namespace std;

class FtraceLogRow;
class FtraceLog;
FtraceLogRow parse_row(string);
vector<FtraceLogRow> parse_log(ifstream&);
const string POSSIBLE_FUNCTIONS_CHARACTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";

class FtraceLogRow{
    private:
        string task;
        int pid;
        int cpu;
        char irqs_off;
        char need_resched;
        char hardirq_softirq;
        char preempt_depth;
        char migrate_disable;
        string timestamp;
        string duration;
        string func;
        string args;
    public:
        // Constructor
        FtraceLogRow(const FtraceLogRow&row)
                    :task(row.task),pid(row.pid),cpu(row.cpu),irqs_off(row.irqs_off),
                    need_resched(row.need_resched),hardirq_softirq(row.hardirq_softirq),
                    preempt_depth(row.preempt_depth),migrate_disable(row.migrate_disable),
                    timestamp(row.timestamp),duration(row.duration),func(row.func),
                    args(row.args){
                        FtraceLogRow();
                    }
        FtraceLogRow(string task, int pid, int cpu,
                    char irqs_off, char need_resched, char hardirq_softirq,
                    char preempt_depth, char migrate_disable, string timestamp,
                    string duration, string func, string args = "")
                    : task(task), pid(pid), cpu(cpu), irqs_off(irqs_off),
                    need_resched(need_resched), hardirq_softirq(hardirq_softirq),
                    preempt_depth(preempt_depth), migrate_disable(migrate_disable),
                    timestamp(timestamp), duration(duration), func(func),
                    args(args) {
                        FtraceLogRow();
                    }
        FtraceLogRow(tuple<string, int, int, char, char, char, char, char, string, string, string, string> row){
            FtraceLogRow(get<0>(row), get<1>(row), get<2>(row), get<3>(row),get<4>(row),get<5>(row),
                        get<6>(row), get<7>(row), get<8>(row), get<9>(row), get<10>(row), get<11>(row));
        }
        FtraceLogRow(tuple<string, int, int, char, char, char, char, char, string, string, string> row){
            FtraceLogRow(get<0>(row), get<1>(row), get<2>(row), get<3>(row),get<4>(row),get<5>(row),
                        get<6>(row), get<7>(row), get<8>(row), get<9>(row), get<10>(row));
        }
        FtraceLogRow(){}

        // Getters
        string get_task(){
            return task;
        }
        int get_pid(){
            return pid;
        }
        int get_cpu(){
            return cpu;
        }
        char get_irqs_off(){
            return irqs_off;
        }
        char get_need_resched(){
            return need_resched;
        }
        char get_hardirq_softirq(){
            return hardirq_softirq;
        }
        char get_preempt_depth(){
            return preempt_depth;
        }
        char get_migrate_disable(){
            return migrate_disable;
        }
        string get_timestamp(){
            return timestamp;
        }
        string get_duration(){
            return duration;
        }
        string get_func(){
            return func;
        }
        string get_args(){
            return args;
        }
        tuple<string, int, int, char, char, char, char, char, string, string, string, string> get_tuple(){
            return {task, pid, cpu, irqs_off, need_resched, hardirq_softirq, preempt_depth, migrate_disable, timestamp, duration, func, args};
        }

        // Setters
        void set_task(string task){
            this->task = task;
        }
        void set_pid(int pid){
            this->pid = pid;
        }
        void set_cpu(int cpu){
            this->cpu = cpu;
        }
        void set_irqs_off(char irqs_off){
            this->irqs_off = irqs_off;
        }
        void set_need_resched(char need_resched){
            this->need_resched = need_resched;
        }
        void set_hardirq_softirq(char hardirq_softirq){
            this->hardirq_softirq = hardirq_softirq;
        }
        void set_preempt_depth(char preempt_depth){
            this->preempt_depth = preempt_depth;
        }
        void set_migrate_disable(char migrate_disable){
            this->migrate_disable = migrate_disable;
        }
        void set_timestamp(string timestamp){
            this->timestamp = timestamp;
        }
        void set_duration(string duration){
            this->duration = duration;
        }
        void set_func(string func){
            this->func = func;
        }
        void set_args(string args){
            this->args = args;
        }
        void set_tuple(tuple<string, int, int, char, char, char, char, char, string, string, string, string> t){
            this->set_task(get<0>(t));
            this->set_pid(get<1>(t));
            this->set_cpu(get<2>(t));
            this->set_irqs_off(get<3>(t));
            this->set_need_resched(get<4>(t));
            this->set_hardirq_softirq(get<5>(t));
            this->set_preempt_depth(get<6>(t));
            this->set_migrate_disable(get<7>(t));
            this->set_timestamp(get<8>(t));
            this->set_duration(get<9>(t));
            this->set_func(get<10>(t));
            this->set_args(get<11>(t));
        }

        // Print
        ostream& print_one(ostream& os) const {
            os << "Task: " << task << '\n'
                << "PID: " << pid << '\n'
                << "CPU: " << cpu << '\n'
                << "IRQs Off: " << irqs_off << '\n'
                << "Need Resched: " << need_resched << '\n'
                << "Hardirq Softirq: " << hardirq_softirq << '\n'
                << "Preempt Depth: " << preempt_depth << '\n'
                << "Migrate Disable: " << migrate_disable << '\n'
                << "Timestamp: " << timestamp << '\n'
                << "Duration: " << duration << '\n'
                << "func: " << func << '\n'
                << "Args: " << args << '\n';
            return os;
        }
        ostream& print_row(ostream& os = cout, string delim = " | ") const {
            os << task << delim << pid << delim 
                << cpu << delim << irqs_off << delim 
                << need_resched << delim << hardirq_softirq << delim 
                << preempt_depth << delim << migrate_disable << delim 
                << timestamp << delim << duration << delim 
                << func << delim << args << '\n';
            return os;
        }
        
        // Operator Overloading
        friend ostream& operator<<(ostream& os, const FtraceLogRow& row){
            return row.print_row(os);
        }
        bool operator<(const FtraceLogRow& row) const {
            return timestamp < row.timestamp;
        }
        bool operator>(const FtraceLogRow& row) const {
            return timestamp > row.timestamp;
        }
        tuple<string, int, int, char, char, char, char, char, string, string, string, string> operator()(){
            return get_tuple();
        }
        string operator[](int i){
            switch(i){
                case 0:
                    return task;
                case 1:
                    return to_string(pid);
                case 2:
                    return to_string(cpu);
                case 3:
                    return to_string(irqs_off);
                case 4:
                    return to_string(need_resched);
                case 5:
                    return to_string(hardirq_softirq);
                case 6:
                    return to_string(preempt_depth);
                case 7:
                    return to_string(migrate_disable);
                case 8:
                    return timestamp;
                case 9:
                    return duration;
                case 10:
                    return func;
                case 11:
                    return args;
                default:
                    throw out_of_range("Index out of range");
            }
        }
        string operator[](string s){
            if(s == "task"){
                return task;
            }
            else if(s == "pid"){
                return to_string(pid);
            }
            else if(s == "cpu"){
                return to_string(cpu);
            }
            else if(s == "irqs_off"){
                return to_string(irqs_off);
            }
            else if(s == "need_resched"){
                return to_string(need_resched);
            }
            else if(s == "hardirq_softirq"){
                return to_string(hardirq_softirq);
            }
            else if(s == "preempt_depth"){
                return to_string(preempt_depth);
            }
            else if(s == "migrate_disable"){
                return to_string(migrate_disable);
            }
            else if(s == "timestamp"){
                return timestamp;
            }
            else if(s == "duration"){
                return duration;
            }
            else if(s == "func"){
                return func;
            }
            else if(s == "args"){
                return args;
            }
            else{
                throw out_of_range("Index out of range");
            }
        }
        string operator()() const {
            stringstream ss;
            ss << task << " | " << pid << " | " 
                << cpu << " | " << irqs_off << " | " 
                << need_resched << " | " << hardirq_softirq << " | " 
                << preempt_depth << " | " << migrate_disable << " | " 
                << timestamp << " | " << duration << " | " 
                << func << " | " << args << '\n';
            return ss.str();
        }
};

class FtraceLog{
    private:
        vector<FtraceLogRow> rows;
    public:
        // Constructors
        FtraceLog(){}
        FtraceLog(vector<FtraceLogRow>&rows){
            this->rows = rows;
        }
        FtraceLog(ifstream&fin){
            this->rows = parse_log(fin);
        }
        FtraceLog(string filename){
            ifstream fin(filename);
            this->rows = parse_log(fin);
        }
        
        // Grouping
        map<string, set<FtraceLogRow>> group_by_task(){
            map<string, set<FtraceLogRow>> grouped;
            for(auto row : rows){
                grouped[row.get_task()].insert(row);
            }
            return grouped;
        }
        map<int, set<FtraceLogRow>> group_by_pid(){
            map<int, set<FtraceLogRow>> grouped;
            for(auto row : rows){
                grouped[row.get_pid()].insert(row);
            }
            return grouped;
        }
        map<int, set<FtraceLogRow>> group_by_cpu(){
            map<int, set<FtraceLogRow>> grouped;
            for(auto row : rows){
                grouped[row.get_cpu()].insert(row);
            }
            return grouped;
        }
        map<char, set<FtraceLogRow>> group_by_irqs_off(){
            map<char, set<FtraceLogRow>> grouped;
            for(auto row : rows){
                grouped[row.get_irqs_off()].insert(row);
            }
            return grouped;
        }
        map<char, set<FtraceLogRow>> group_by_need_resched(){
            map<char, set<FtraceLogRow>> grouped;
            for(auto row : rows){
                grouped[row.get_need_resched()].insert(row);
            }
            return grouped;
        }
        map<char, set<FtraceLogRow>> group_by_hardirq_softirq(){
            map<char, set<FtraceLogRow>> grouped;
            for(auto row : rows){
                grouped[row.get_hardirq_softirq()].insert(row);
            }
            return grouped;
        }
        map<char, set<FtraceLogRow>> group_by_preempt_depth(){
            map<char, set<FtraceLogRow>> grouped;
            for(auto row : rows){
                grouped[row.get_preempt_depth()].insert(row);
            }
            return grouped;
        }
        map<char, set<FtraceLogRow>> group_by_migrate_disable(){
            map<char, set<FtraceLogRow>> grouped;
            for(auto row : rows){
                grouped[row.get_migrate_disable()].insert(row);
            }
            return grouped;
        }
        map<string, set<FtraceLogRow>> group_by_timestamp(){
            map<string, set<FtraceLogRow>> grouped;
            for(auto row : rows){
                grouped[row.get_timestamp()].insert(row);
            }
            return grouped;
        }
        map<string, set<FtraceLogRow>> group_by_duration(){
            map<string, set<FtraceLogRow>> grouped;
            for(auto row : rows){
                grouped[row.get_duration()].insert(row);
            }
            return grouped;
        }
        map<string, set<FtraceLogRow>> group_by_func(){
            map<string, set<FtraceLogRow>> grouped;
            for(auto row : rows){
                grouped[row.get_func()].insert(row);
            }
            return grouped;
        }
        map<string, set<FtraceLogRow>> group_by_args(){
            map<string, set<FtraceLogRow>> grouped;
            for(auto row : rows){
                grouped[row.get_args()].insert(row);
            }
            return grouped;
        }
};



FtraceLogRow parse_row(string line){
    stringstream ss(line);
    FtraceLogRow row;
    string tmp;
    ss >> tmp;
    auto last_dash = tmp.find_last_of('-');
    row.set_task(tmp.substr(0, last_dash-1));
    row.set_pid(stoi(tmp.substr(last_dash+1)));
    ss >> tmp;
    row.set_cpu(stoi(tmp.substr(1, tmp.size()-2)));
    ss >> tmp;
    row.set_irqs_off(tmp[0]);
    row.set_need_resched(tmp[1]);
    row.set_hardirq_softirq(tmp[2]);
    row.set_preempt_depth(tmp[3]);
    row.set_migrate_disable(tmp[4]);
    ss >> tmp;
    row.set_timestamp(tmp.substr(0, tmp.size()-1));
    row.set_duration("");
    ss >> tmp;
    auto first_arg = tmp.find_first_not_of(POSSIBLE_FUNCTIONS_CHARACTERS);
    row.set_func(tmp.substr(first_arg-1));
    row.set_args(tmp.substr(first_arg));
    return row;
}
vector<FtraceLogRow> parse_log(ifstream& fin){
    vector<FtraceLogRow> rows;
    string str;
    while(getline(fin,str)){
        if(str[0] == '#'){
            continue;
        }
        else{
            rows.push_back(parse_row(str));
        }
    }
    return rows;
}

int main(int argc, char *argv[]) {
    if(argc < 2){
        cout << "Usage: sudo ./ftrace_extract <ftrace_file>" << endl;
        return 0;
    }
    ifstream fin(argv[1]);
    ofstream fout("ftrace_extract.txt");
    string str;
    int i=0;
    FtraceLog log(fin);
    auto grouped = log.group_by_func();
    for(auto it : grouped){
        fout << it.first << " " << it.second.size() << endl;
    }


    return 0;

}