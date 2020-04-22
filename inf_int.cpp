#include "inf_int.h"

#define MAX 10000        //buf�迭�� ���̴� �˳��� 10000���� ���ش�.

inf_int::inf_int()
{
	this->digits = new char[2];	// �����Ҵ�

	this->digits[0] = '0';		// default �� 0 ����
	this->digits[1] = '\0';
	this->length = 1;
	this->thesign = true;
}

inf_int::inf_int(int n) {
	char buf[MAX];

	if (n<0) {		// ���� ó��
		this->thesign = false;
		n = -n;
	}
	else {
		this->thesign = true;
	}

	int i = 0;
	while (n>0) {			// ���ڸ� ���ڿ��� ��ȯ�ϴ� ����
		buf[i] = n % 10 + '0';

		n /= 10;
		i++;
	}

	if (i == 0) {	// ������ ������ 0�� ���
		new (this) inf_int();	// ������ ��ȣ��...gcc���� �����Ͽ����� �ִٰ� ��. inf_int()�� ��� ������ �ν��Ͻ��� ������. 
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
	int len= strlen(str); //���ڿ��� ����
	char buf[MAX];        //���� �������� �迭����


	//������ ��.
	if (str[0] == '-') {
		this->thesign = false;
		for (int i = len-1 ; i > 0; i--)    //�ε��� 0���� ������ �ʴ´�. '-'�ڸ������Ƿ�.
				buf[len - 1 - i] = str[i];
		buf[len-1] = '\0';
		len = len - 1;
	}
	//����� ��.
	else {
		this->thesign = true;
		for (int i = len-1 ; i >= 0; i--)    //�ε��� 0���� ������. 
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
	delete digits;		// �޸� �Ҵ� ����
}

inf_int& inf_int::operator=(const inf_int& a)
{
	if (this->digits) {
		delete this->digits;		// �̹� ���ڿ��� ���� ��� ����.
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
	if ((strcmp(a.digits, b.digits) == 0) && a.thesign == b.thesign)	// ��ȣ�� ����, ������ ��ġ�ؾ���.
		return true;
	return false;
}

bool operator!=(const inf_int& a, const inf_int& b)
{
	return !operator==(a, b);
}



bool operator>(const inf_int& a, const inf_int& b)
{
	//��ȣ�� �ٸ����
	if (a.thesign != b.thesign) {  
		if (a.thesign == false)    //a�� �����̸� false.
			return false;
		else                       //b�� �����̸� true.
			return true;
	}
	//��ȣ�� �������
	else{
		bool sign;
		if (a.length > b.length)
			sign = true;
		else if (a.length < b.length)
			sign = false;
		else {
			//a�� ������ �����ϰ� �� ��. ���߿� a�� ������� �ݴ밪�� return ���� ����.
			int len = a.length;
			for (int i = len - 1; i >= 0; i--) {
				int left = a.digits[i] - '0';                      //�ƽ�Ű �ڵ� �̿��Ͽ� ����->���� �ٲٱ�.
				int right = b.digits[i] - '0';
				if (left > right) { sign = true; break;}           //left���� ó������ ū���� ������ true�� �����ϰ� break.
				else if (left < right) {sign = false; break;}      //right���� ó������ ū���� ������ true�� �����ϰ� break.
				else if (i == 0 && left == right) return false;    //for���� ������ ���ư��ٸ� ���� ������ ���� �� �̹Ƿ� false����.
				else continue;

			}
		}

		if (a.thesign == true) { return sign; } //a�� ������ �״�� ��ȯ.
		else { return !sign; }                  //a�� ������� �ݴ�� ��ȯ.

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


	//a,b ��ȣ�� ���ٸ� ���� �����ִ� ���� ���� ����. 
	if (a.thesign == b.thesign) {                         
		int i, next = 0, now = 0, right = 0, left = 0;

		if (b.thesign) sign = true;                       //�Ѵ� ������ ���Ѱ��� ���.
		else sign = false;                                //�Ѵ� ������� ���Ѱ��� ����.

		if (a.length >= b.length) len = a.length;         //�� �߿� ���̰� �� ���� ���� len�� �������ش�.
		else len = b.length;


		for (i = 0; i < len; i++) {
			if (a.digits[i] == '\0' || b.digits[i] == '\0')
				break;
			right = a.digits[i] - '0';   //����->����
			left = b.digits[i] - '0';
			now = right + left + next;    
			next = 0;
			if (now >= 10) {             //������ ���� 10�� �Ѵ´ٸ� now���� �����ڸ��� ���� next���� +1���Ѽ� ���� ���ͷ��̼ǿ� �����ش�.
				now = now % 10;
				next += 1;
			}
			buf[i] = now + '0';
			if (a.digits[i + 1] == '\0' && b.digits[i + 1] == '\0' && next == 1) { //���� ���̰� ����, ������ ���ڸ� ���ߴµ� 10�̻��� ��츦 ����Ͽ� ���� �ε����� �־��ְ� ����.
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
	//a,b ��ȣ�� �ٸ��ٸ� ���� �����ִ� ���� ���� ����. 
	else {
		inf_int big, small;
		int i, next = 0, upper, lower, now;
		if (a.length > b.length) {

			for (i = 0; i < b.length; i++) {
				upper = a.digits[i] - '0'; //����->����
				lower = b.digits[i] - '0';
				if (upper - next >= lower) {                    //�ε��� i������ a�� ���ڰ��� b�� ���ڰ����� ���ų� ū��� �׳� ���ش�.
					now = upper - lower - next;
					next = 0;
				}
				else {                                         //�ε��� i������ a�� ���ڰ��� b�� ���ڰ����� ���ų� ������� a�� 10�� ���ϰ� b�� ���ش�. ��� next�� +1�� ���־� ���� ���ͷ��̼Ƕ� ���ش�.
					now = upper + 10 - lower - next;
					next = 1;
				}
				buf[i] = now + '0';
			}
			for (i = b.length; i < a.length; i++) {             //�� ���ְ� ���� ���ڵ��� �״�� buf�� �������ش�. �ٸ� ���� next�� 0�� �ƴ� ��츦 ����� ���ش�.
				now = a.digits[i] - '0';
				now = now - next;
				next = 0;
				if (a.digits[i + 1] == '\0' && now == 0) {      //next�� ���ָ� ���� ū�ڸ��� 0�ϼ� �����Ƿ� �׷���� len�� �ϳ� �ٿ��ش�.
					len = a.length - 1;
					break;
				}
				if (a.digits[i + 1] == '\0' && now != 0) {
					len = a.length;
				}
				buf[i] = now + '0'; //����->����
			}
			sign = a.thesign;


		}
		else if (a.length < b.length) {              //���� ������ b�� ū���.
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
			len = a.length; //a�� b�� ���̴� ���� ������ �� �߿� ��� ���� �־��൵ ������.

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
				else if (i == len - 1 && left == right) {  			//a�� b�� ������ ���� ��ȣ�� �ٸ��� ���ϸ� 0�̹Ƿ� �׳� 0 ��ȯ.
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
				upper = big.digits[i] - '0';   //����->����
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

			//���� ū �ڸ����ڰ� 0�� ���ð�� 0�� �������ִ� for��.
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

	//a,b ��ȣ�� �ٸ��ٸ� ���� �����ִ� ���� ����. 
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
	//a,b ��ȣ�� ���ٸ� �����Ͽ� ���ִ� ���� ����.
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
		else if (a.length < b.length) {                     //���� ������ b�� ū ���.
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
			len = a.length; //a�� b�� ���̴� ���� ������ �� �߿� ��� ���� �־��൵ ������.


			//������ �����Ͽ��� operator < �Լ��� �̿��Ͽ� ��ȣ�� big,small �� �������ش�.
			if (a.thesign == false && a<b) { big = a; small = b; sign = true; }
			else if (a.thesign == true && a < b) { big = b; small = a; sign = false; }
			else if (a.thesign == true && a > b) { big = a; small = b; sign = true; }
			else if (a.thesign == false && a > b) { big = b; small = a; sign = false; }
			//a�� b�� ���ٸ� ���� 0�̹Ƿ� �׳� 0 ��ȯ.
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

			//���� ū �ڸ����ڰ� 0�� ���ð�� 0�� �������ִ� for��.
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
	int temp[MAX-1] = { 0, }; //int �� �迭�̹Ƿ� '\0'�� �־��� �ʿ䰡 �����Ƿ� MAX���� 1���� �迭 ����. �� �������� ���ڷ� ��ȯ�Ͽ� buf�� �������� ��.
	inf_int c;

	if (a.thesign == b.thesign) { sign = true; }
	else { sign = false; }

	for (int i = 0; i < b.length; i++) {         // �ΰ��� �ε����� �����Ͽ����ϰ�, ������ �̷������ �ϹǷ� ����for�� ���.
		upper = b.digits[i] - '0';               // b�� �ڸ����� a�� �����ֱ� ������ b�� �������ְ� a�� ��ȭ���Ѱ��� �����ش�. ����->����
		for (int j = 0; j < a.length; j++) {
			lower = a.digits[j] - '0';

			now = upper * lower;                 //�ΰ��� ��.

			if (now >= 10) {                     //������ ������ ���� 10���� Ŭ��� �������� ���� �ε����� �������ְ�,���� ���� �ε�����  ���� ����.
				temp[j + i + 1] += now/10;
				temp[j + i] += now % 10;
			}
			else {                                // 10���� ������� �׳� �����ش�.
				temp[j + i] += now % 10;
			}
			if (temp[i + j] >= 10) {                       //���ͷ��̼��� �ݺ��ǰ� �������� �����ִٺ��� ���� �ε����� ���� 10�� �Ѵ� ��찡 �߻��ϴµ� �� ��� ���.
				temp[j + i + 1] += temp[j + i] / 10;
				temp[j + i] = temp[j + i] % 10;
			}


		}
	}

	for (int i = MAX-2; i >= 0; i--) {             //ó������ 0�̾ƴ� ���ڰ� ���°�찡 ���̰��ȴ�.
		if (temp[i] != 0) { 
			len = i+1;
			break;
		}
		len = 1;
	}

	for (int i = 0; i < len; i++) { buf[i] = temp[i] + '0';}   //�������迭�� ���ڷ� ��ȯ�Ͽ� buf �迭�� �������ش�.

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