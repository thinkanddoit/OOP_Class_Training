#include <iostream> 
#include <stdexcept>

class Test {
public:
	Test(char id) : id_(id) {}
	~Test() { std::cout << "Destructor execution: " << id_ << std::endl; }
private:
	char id_;
};
int funcB() {
	Test r('B');
	throw std::runtime_error("Exception from funcB!\n");
	std::cout << "Executed in B" << std::endl;
	return 0;
}
int funcA() {
	Test r('A');
	funcB();
	std::cout << "Executed in A" << std::endl;
	return 0;
}

int main() {
	try {
		funcA();
	}
	catch (std::exception & e) {
		std::cout << "Exception : " << e.what();
	}
}

/* (실습 1-1) 분석결과 :
<콘솔 창 결과>
Executed in B
Destructor execution : B
Executed in A
Destructor execution : A

첫번째 주어진 코드를 실행하고나서 위와같은 결과가 나온 이유는
우선 main문에서 try구문안에서 funcA가 실행이된다.
funcA에서는 Test객체를 생성하고 funcB를 실행한다.
funcB에서는 마찬가지로 Test객체를 생성하고 "Executed in B"라는 첫번째 출력이 실행된다.
다음 줄에서 0을 return 하면서 함수가 종료되고 ~Test()소멸자가 실행되면서
"Destructor execution: B(id값)"라는 두번째 출력이 실행된다.
이후 다시 funcA로 돌아가서 앞의 방식과 똑같이
"Executed in A", "Destructor execution : A" 라는 세번째, 네번째 출력이 실행되며 예외처리없이 프로그램은 종료된다.
*/

/* (실습 1-2) 분석결과 :
<콘솔 창 결과>
Destructor execution: B
Destructor execution: A
Exception : Exception from funcB!

throw의 주석(//)을 해제한 후 다음과 같은 결과가 나온 이유는
Stack unwinding이 발생했기 때문이다.

Stack unwinding은 C++예외처리에서는 예외를 던지는 throw문은 try구문안에 있어야하고
throw 발생시 예외를 받아주는 catch구문으로 점프하게된다.
그러나 함수내에서 try, catch 구문없이 throw만 발생시키는 함수가 있으면 catch구문을찾기위해
함수 외부로 나간다. 예외를 던진 함수에서 외부로 나가면서 현재 스택정보를 정리하고 빠져나가는
Stack unwinding이 이때 발생하게된다.

위의 코드를 분석해보면
funcA 실행 -> funcB 실행 중 throw 발생 catch문을 찾으면서 스택 정리
-> funcB의 소멸자가 불러지면서 "Destructor execution: B" 출력문 발생
-> funcA로 돌아와서 역시 catch문을 찾으면서 스택 정리
-> funcA의 소멸자가 불러지면서 "Destructor execution: A" 출력문 발생
-> catch 문을 못찾아서 main()함수까지 돌아옴
-> catch문 찾음 -> Exception : Exception from funcB! 예외 출력문 발생
-> 프로그램 종료
*/