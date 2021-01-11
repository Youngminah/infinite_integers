# Infinite Integer Calculator 

This is the one of my project of Object-Oriented Programming. I used C++ to create this calculator. It is designed in order to calculate decimal using four basic arithmetic operators. 


## :bulb: Rule of this Projects.

```console
- Complete implementing inf_int class that represents infinite precision integer and its operations. The specification of inf_int is provided to you in "inf_int.h". You should add its implementation in "inf_int.cpp". 
- Test the correctness of your implementation by writing "main.cpp". "inf_int.h" and an example of "main.cpp" are downloadable from our class website. After compilation, your code should generate correct result. 
- The result of this problem should be submitting compilable source code package (inf_int.h, inf_int.cpp, main.cpp, .sln files (files for opening with visual studio 2010 or 20xx)).
- You have to effectively use dynamic memory allocation(new) and deallocation(delete) for handling the variable sizes of infinite precision integer numbers.
```


## :point_up: Submission Rule.

1. Create a directory "proj2".
2. Insert compilable source code package(inf_int.h, inf_int.cpp, main.cpp, ...) and readme.txt into “proj2”. In readme.txt file, you should briefly explain how to compile and execute the source codes you submit.
3. zip the directory "proj2" into proj2.zip and submit the zip file into eClass homework board.


## :paperclip: Main Code Review

Main.cpp is very simple. I put a big integer to test whether this project is working perfectly or not.
Here's the code.

<pre><code>
int main()
{
	inf_int a, e;
  //*********here*************
	inf_int b(100);
	inf_int c("321111111111122222222222233333333333444444444445555555555");     
	inf_int d("123451987651234572749499923455022211");
	inf_int f = d;
	inf_int g(f);
  //**************************
  
  
	// cin >> g ;   // not required

	a = b * c;
	// e=g/f;       // not required

	b = c - d;


	if (f == d) {
		cout << "a : " << a << endl;
		cout << "b : " << b << endl;
		cout << "c : " << c << endl;
		cout << "d : " << d << endl;
		cout << "f : " << f << endl;
	}

	return 0;
}
</code></pre>

## :sunflower: Result DEMO.
![image](https://user-images.githubusercontent.com/42762236/80022972-cd1a0d00-8517-11ea-8095-159ed8f96fdd.png)

The result of integer is around 10^61~ 10^62. Therefore, it shows that the project works well.  
If you want to operate another integers, You can change the main.cpp whatever you want.


## :mailbox_with_mail: Connect

```
  Github : https://github.com/Youngminah
  Email : kimym7418@gmail.com
  Website : https://cau-meng2.tistory.com/
```

