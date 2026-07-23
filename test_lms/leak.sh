# 1) Build KHÔNG dùng ASan
g++ -g -std=c++11 main.cpp dragon.cpp -I . -o main

# 2) Bật stack logging và tắt nano allocator cho trace rõ
export MallocStackLogging=1
export MallocNanoZone=0

# 3) Chạy chương trình ở background và giữ nó sống 1 chút (ví dụ bằng sleep cuối chương trình)
./main & pid=$!

# 4) Attach leaks để lấy stack trace các allocation chưa free
leaks -trace $pid

# 5) (tuỳ chọn) xem các block theo size
leaks --list $pid
