LibEventStu:LibEventStu.cpp
g++ $^ -o $@ -levent
./$@
clean:
rm -rf LibEventStu
rm -rf *.o