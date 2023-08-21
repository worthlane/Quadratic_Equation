CXX = g++-13
TARGET = quad_eq
CXXFLAGS =  -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla
BUILD = $(CXX) -c $^ -o $@ $(CXXFLAGS)

.PHONY: all
all: $(TARGET)

$(TARGET): main.o solver.o getout_info.o run_modes.o
	$(CXX) $^ -o $(TARGET) $(CXXFLAGS)

main.o: main.cpp solver.h getout_info.h run_modes.h
	$(BUILD)

run_modes.o: run_modes.cpp solver.h run_modes.h getout_info.h
	$(BUILD)

getout_info.o: getout_info.cpp getout_info.h solver.h
	$(BUILD)

solver.o: solver.cpp solver.h
	$(BUILD)

.PHONY: clean
clean:
	rm -rf $(TARGET) *.d *.o

