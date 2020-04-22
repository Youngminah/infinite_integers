#include "inf_int.h"

#define MAX 10000        //buf배열의 길이는 넉넉히 10000으로 해준다.

inf_int::inf_int()
{
	this->digits = new char[2];	// 동적할당

	this->digits[0] = '0';		// default 값 0 설정
	this->digits[1] = '\0';
	this->length = 1;
	this->thesign = true;
}

inf_int::inf_int(int n) {
	char buf[MAX];

	if (n<0) {		// 음수 처리
		this->thesign = false;
		n = -n;
	}
	else {
		this->thesign = true;
	}

	int i = 0;
	while (n>0) {			// 숫자를 문자열로 변환하는 과정
		buf[i] = n % 10 + '0';

		n /= 10;
		i++;
	}

	if (i == 0) {	// 숫자의 절댓값이 0일 경우
		new (this) inf_int();	// 생성자 재호출...gcc에서 컴파일에러가 있다고 함. inf_int()의 경우 별개의 인스턴스가 생성됨. 
	}
	else {
		buf[i] = '\0';

		this->digits = new char[i + 1];
		this->length = i;
		strcpy(this->digits, buf);
	}
}

inf_int::inf_int(const char* str)
{
	int len= strlen(str); //문자열의 길이
	char buf[MAX];        //문자 저장해줄 배열선언


	//음수일 떄.
	if (str[0] == '-') {
		this->thesign = false;
		for (int i = len-1 ; i > 0; i--)    //인덱스 0까지 돌리지 않는다. '-'자리였으므로.
				buf[len - 1 - i] = str[i];
		buf[len-1] = '\0';
		len = len - 1;
	}
	//양수일 떄.
	else {
		this->thesign = true;
		for (int i = len-1 ; i >= 0; i--)    //인덱스 0까지 돌린다. 
			buf[len - 1 - i] = str[i];
		buf[len] = '\0';
	}

	this->digits = new char[len + 1];
	this->length = len;
	strcpy(this->digits, buf);
}

inf_int::inf_int(const inf_int& a) {
	this->digits = new char[a.length + 1];

	strcpy(this->digits, a.digits);
	this->length = a.length;
	this->thesign = a.thesign;
}

inf_int::~inf_int() {
	delete digits;		// 메모리 할당 해제
}

inf_int& inf_int::operator=(const inf_int& a)
{
	if (this->digits) {
		delete this->digits;		// 이미 문자열이 있을 경우 제거.
	}
	this->digits = new char[a.length + 1];

	strcpy(this->digits, a.digits);
	this->length = a.length;
	this->thesign = a.thesign;

	return *this;
}

bool operator==(const inf_int& a, const inf_int& b)
{
	// we assume 0 is always positive.
	if ((strcmp(a.digits, b.digits) == 0) && a.thesign == b.thesign)	// 부호가 같고, 절댓값이 일치해야함.
		return true;
	return false;
}

bool operator!=(const inf_int& a, const inf_int& b)
{
	return !operator==(a, b);
}



bool operator>(const inf_int& a, const inf_int& b)
{
	//부호가 다른경우
	if (a.thesign != b.thesign) {  
		if (a.thesign == false)    //a가 음수이면 false.
			return false;
		else                       //b가 음수이면 true.
			return true;
	}
	//부호가 같은경우
	else{
		bool sign;
		if (a.length > b.length)
			sign = true;
		else if (a.length < b.length)
			sign = false;
		else {
			//a가 양수라고 생각하고 쓴 식. 나중에 a가 음수라면 반대값을 return 해줄 것임.
			int len = a.length;
			for (int i = len - 1; i >= 0; i--) {
				int left = a.digits[i] - '0';                      //아스키 코드 이용하여 문자->숫자 바꾸기.
				int right = b.digits[i] - '0';
				if (left > right) { sign = true; break;}           //left값이 처음으로 큰수가 나오면 true를 저장하고 break.
				else if (left < right) {sign = false; break;}      //right값이 처음으로 큰수가 나오면 true를 저장하고 break.
				else if (i == 0 && left == right) return false;    //for문이 끝까지 돌아간다면 둘이 완전히 같은 수 이므로 false리턴.
				else continue;

			}
		}

		if (a.thesign == true) { return sign; } //a가 양수라면 그대로 반환.
		else { return !sign; }                  //a가 음수라면 반대로 반환.

	}

}

bool operator<(const inf_int& a, const inf_int& b)
{
	if (operator>(a, b) || operator==(a, b)) { return false; }
	else {return true;}
}

inf_int operator+(const inf_int& a, const inf_int& b)       
{
	char buf[1000];
	bool sign;
	int len;
	inf_int result;


	//a,b 부호가 같다면 누적 더해주는 식의 구문 구현. 
	if (a.thesign == b.thesign) {                         
		int i, next = 0, now = 0, right = 0, left = 0;

		if (b.thesign) sign = true;                       //둘다 양수라면 더한값도 양수.
		else sign = false;                                //둘다 음수라면 더한값도 음수.

		if (a.length >= b.length) len = a.length;         //둘 중에 길이가 긴 것을 변수 len에 저장해준다.
		else len = b.length;


		for (i = 0; i < len; i++) {
			if (a.digits[i] == '\0' || b.digits[i] == '\0')
				break;
			right = a.digits[i] - '0';   //문자->숫자
			left = b.digits[i] - '0';
			now = right + left + next;    
			next = 0;
			if (now >= 10) {             //더해준 값이 10이 넘는다면 now에는 일의자리만 저장 next에는 +1시켜서 다음 이터레이션에 더해준다.
				now = now % 10;
				next += 1;
			}
			buf[i] = now + '0';
			if (a.digits[i + 1] == '\0' && b.digits[i + 1] == '\0' && next == 1) { //둘의 길이가 같고, 마지막 숫자를 더했는데 10이상일 경우를 대비하여 다음 인덱스에 넣어주고 종료.
				buf[i + 1] = next + '0';
				len += 1;
				i++;
				break;
			}
		}

		for (; i < len; i++) {
			if (a.length > b.length) {
				buf[i] = a.digits[i] + next;
			}
			else if (a.length < b.length) {
				buf[i] = b.digits[i] + next;
			}
			next = 0;
		}
	} 
	//a,b 부호가 다르다면 누적 더해주는 식의 구문 구현. 
	else {
		inf_int big, small;
		int i, next = 0, upper, lower, now;
		if (a.length > b.length) {

			for (i = 0; i < b.length; i++) {
				upper = a.digits[i] - '0'; //문자->숫자
				lower = b.digits[i] - '0';
				if (upper - next >= lower) {                    //인덱스 i에서의 a의 숫자값이 b의 숫자값보다 같거나 큰경우 그냥 빼준다.
					now = upper - lower - next;
					next = 0;
				}
				else {                                         //인덱스 i에서의 a의 숫자값이 b의 숫자값보다 같거나 작은경우 a에 10을 더하고 b를 빼준다. 대신 next에 +1을 해주어 다음 이터레이션때 빼준다.
					now = upper + 10 - lower - next;
					next = 1;
				}
				buf[i] = now + '0';
			}
			for (i = b.length; i < a.length; i++) {             //다 뺴주고 남은 숫자들은 그대로 buf에 복사해준다. 다만 이전 next가 0이 아닐 경우를 대비해 빼준다.
				now = a.digits[i] - '0';
				now = now - next;
				next = 0;
				if (a.digits[i + 1] == '\0' && now == 0) {      //next를 빼주면 가장 큰자리가 0일수 있으므로 그런경우 len를 하나 줄여준다.
					len = a.length - 1;
					break;
				}
				if (a.digits[i + 1] == '\0' && now != 0) {
					len = a.length;
				}
				buf[i] = now + '0'; //숫자->문자
			}
			sign = a.thesign;


		}
		else if (a.length < b.length) {              //위와 같으나 b가 큰경우.
			for (i = 0; i < a.length; i++) {
				upper = b.digits[i] - '0';
				lower = a.digits[i] - '0';
				if (upper - next >= lower) {
					now = upper - lower - next;
					next = 0;
				}
				else {
					now = upper + 10 - lower - next;
					next = 1;
				}
				buf[i] = now + '0';
			}
			for (i = a.length; i < b.length; i++) {
				now = b.digits[i] - '0';
				now = now - next;
				next = 0;
				if (b.digits[i + 1] == '\0' && now == 0) {
					len = b.length - 1;
					break;
				}
				if (b.digits[i + 1] == '\0' && now != 0) {
					len = b.length;
				}
				buf[i] = now + '0';
			}

			sign = b.thesign;
		}
		else {
			len = a.length; //a와 b의 길이는 같기 때문에 둘 중에 어느 것을 넣어줘도 무방함.

			for (int i = 0; i < len; i++) {
				int left, right;
				left = a.digits[i] - '0';
				right = b.digits[i] - '0';
				if (left> right) {
					big = a;
					small = b;
					sign = a.thesign;
				}
				else if (left < right) {
					big = b;
					small = a;
					sign = b.thesign;
				}
				else if (i == len - 1 && left == right) {  			//a와 b가 절댓값이 같고 부호가 다르면 더하면 0이므로 그냥 0 반환.
					buf[0] = '0';
					buf[1] = '\0';
					result.thesign = true;
					result.digits = new char[2];
					result.length = 1;
					strcpy(result.digits, buf);
					return result;
				}

			}


			for (int i = 0; i < big.length; i++) {
				upper = big.digits[i] - '0';   //문자->숫자
				lower = small.digits[i] - '0';
				if (upper - next >= lower) {
					now = upper - lower - next;
					next = 0;
				}
				else {
					now = upper + 10 - lower - next;
					next = 1;
				}
				buf[i] = now + '0';
			}

			//가장 큰 자리숫자가 0이 나올경우 0은 생략해주는 for문.
			for (int j = len - 1; j >= 0; j--) {
				if (buf[j] == '0') { buf[j] = '\0'; len -= 1; }
				else { break; }
			}
		}

	}

	buf[len] = '\0';
	result.thesign = sign;
	result.digits = new char[len + 1];
	result.length = len;
	strcpy(result.digits, buf);


	return result;
}


inf_int operator-(const inf_int& a, const inf_int& b)
{
	char buf[1000];
	bool sign;
	int len;
	inf_int result;

	//a,b 부호가 다르다면 누적 더해주는 구문 구현. 
	if (a.thesign != b.thesign) {
		int i, next=0, now = 0, right = 0, left = 0;

		if (b.thesign) sign = false;
		else sign = true;

		if (a.length >= b.length) len = a.length;
		else len = b.length;

		for (i = 0; i < len; i++) {
			if (a.digits[i] == '\0' || b.digits[i] == '\0')
				break;
			right = a.digits[i] - '0';
			left = b.digits[i] - '0';
			now = right + left+ next;
			next = 0;
			if (now >= 10) {
				now = now % 10;
				next += 1;
			}
			buf[i] = now + '0';
			if (a.digits[i + 1] == '\0' && b.digits[i + 1] == '\0' && next == 1) {
				buf[i+1] = next + '0';
				len += 1;
				i++;
				break;
			}
		}

		for (; i < len; i++) {
			if (a.length > b.length) {
				buf[i] = a.digits[i]+next;
			}
			else if (a.length < b.length){
				buf[i] = b.digits[i]+next;
			}
			next = 0;
		}
	}
	//a,b 부호가 같다면 누적하여 빼주는 구문 구현.
	else {
		inf_int big, small;
		int i, next = 0, upper, lower, now;
		if (a.length > b.length) {

			for (i = 0; i < b.length; i++) {
				upper = a.digits[i] - '0';
				lower = b.digits[i] - '0';
				if (upper-next >= lower) { 
					now = upper - lower-next;
					next = 0;
				}
				else {
					now = upper + 10 - lower-next;
					next = 1;
				}
				buf[i] = now + '0';
			}
			for (i = b.length; i < a.length; i++) {
				now = a.digits[i] - '0';
				now = now - next;
				next = 0;
				if (a.digits[i + 1] == '\0' && now == 0) {
					len = a.length - 1;
					break;
				}
				if (a.digits[i + 1] == '\0' && now != 0) {
					len = a.length ;
				}
				buf[i] = now + '0';
			}
			sign = a.thesign; 


		}
		else if (a.length < b.length) {                     //위와 같으나 b가 큰 경우.
			for (i = 0; i < a.length; i++) {
				upper = b.digits[i] - '0';
				lower = a.digits[i] - '0';
				if (upper - next >= lower) {
					now = upper - lower - next;
					next = 0;
				}
				else {
					now = upper + 10 - lower - next;
					next = 1;
				}
				buf[i] = now + '0';
			}
			for (i = a.length; i < b.length; i++) {
				now = b.digits[i] - '0';
				now = now - next;
				next = 0;
				if (b.digits[i + 1] == '\0' && now == 0) {
					len = b.length - 1;
					break;
				}
				if (b.digits[i + 1] == '\0' && now != 0) {
					len = b.length;
				}
				buf[i] = now + '0';
			}

			sign = b.thesign; 
		}
		else {
			len = a.length; //a와 b의 길이는 같기 때문에 둘 중에 어느 것을 넣어줘도 무방함.


			//위에서 구현하였던 operator < 함수를 이용하여 부호와 big,small 을 결정해준다.
			if (a.thesign == false && a<b) { big = a; small = b; sign = true; }
			else if (a.thesign == true && a < b) { big = b; small = a; sign = false; }
			else if (a.thesign == true && a > b) { big = a; small = b; sign = true; }
			else if (a.thesign == false && a > b) { big = b; small = a; sign = false; }
			//a와 b가 같다면 빼면 0이므로 그냥 0 반환.
			else if (a == b) {
				buf[0] = '0';
				buf[1] = '\0';
				result.thesign = true;
				result.digits = new char[2];
				result.length = 1;
				strcpy(result.digits, buf);
				return result;
			}

			for (int i = 0; i < big.length; i++) {
				upper = big.digits[i] - '0';
				lower = small.digits[i] - '0';
				if (upper - next >= lower) {
					now = upper - lower - next;
					next = 0;
				}
				else {
					now = upper + 10 - lower - next;
					next = 1;
				}
				buf[i] = now + '0';
			}

			//가장 큰 자리숫자가 0이 나올경우 0은 생략해주는 for문.
			for (int j = len - 1; j >= 0; j--) {
				if (buf[j] == '0') {buf[j] = '\0'; len -= 1;}
				else {break;}
			}
		}

	}

	buf[len] = '\0';
	result.thesign = sign;
	result.digits = new char[len + 1];
	result.length = len;
	strcpy(result.digits, buf);


	return result;

}

inf_int operator*(const inf_int& a, const inf_int& b)
{
	// to be filled
	char buf[MAX];
	bool sign;
	int len, upper, lower, now, mul_next = 0, add_next = 0;;
	int temp[MAX-1] = { 0, }; //int 형 배열이므로 '\0'을 넣어줄 필요가 없으므로 MAX보다 1작은 배열 선언. 다 더해준후 문자로 변환하여 buf에 저장해줄 것.
	inf_int c;

	if (a.thesign == b.thesign) { sign = true; }
	else { sign = false; }

	for (int i = 0; i < b.length; i++) {         // 두개의 인덱스를 접근하여야하고, 곱셈이 이루어져야 하므로 이중for문 사용.
		upper = b.digits[i] - '0';               // b의 자리들은 a에 곱해주기 떄문에 b를 고정해주고 a를 변화시켜가며 곱해준다. 문자->숫자
		for (int j = 0; j < a.length; j++) {
			lower = a.digits[j] - '0';

			now = upper * lower;                 //두개의 곱.

			if (now >= 10) {                     //위에서 곱해준 곱이 10보다 클경우 나머지는 현재 인덱스에 저장해주고,몫은 다음 인덱스에  누적 저장.
				temp[j + i + 1] += now/10;
				temp[j + i] += now % 10;
			}
			else {                                // 10보다 작은경우 그냥 더해준다.
				temp[j + i] += now % 10;
			}
			if (temp[i + j] >= 10) {                       //이터레이션이 반복되고 누적으로 더해주다보면 현재 인덱스에 값이 10이 넘는 경우가 발생하는데 이 경우 대비.
				temp[j + i + 1] += temp[j + i] / 10;
				temp[j + i] = temp[j + i] % 10;
			}


		}
	}

	for (int i = MAX-2; i >= 0; i--) {             //처음으로 0이아닌 숫자가 나온경우가 길이가된다.
		if (temp[i] != 0) { 
			len = i+1;
			break;
		}
		len = 1;
	}

	for (int i = 0; i < len; i++) { buf[i] = temp[i] + '0';}   //정수형배열을 문자로 변환하여 buf 배열에 저장해준다.

	buf[len] = '\0';
	c.thesign = sign;
	c.digits = new char[len + 1];
	c.length = len;
	strcpy(c.digits, buf);

	return c;

}


ostream& operator<<(ostream& out, const inf_int& a)
{
	int i;

	if (a.thesign == false) {out << '-';}
	for (i = a.length - 1; i >= 0; i--) {out << a.digits[i];}
	return out;
}