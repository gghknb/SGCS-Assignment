1)�й�:20141575
   Student Id : 20141575

2)�̸�:������
   Name : Hyungsuk Lim

3)������Ʈ ���� ȯ��: �������α׷� :putty(����:cspro.sogang.ac.kr), visual studio 2010
���߰���: r912 ,k512, �� ��Ʈ��
   Project environment : Putty(server : cspro.sogang.ac.kr), visual studio 2010
   Development Place : Sogang University R912 lab , K512 lab , Notebook  
4)������ ���:
 putty���� vi project.c �� �ۼ����� math library �� �̿��ϱ� ���� gcc �ڿ� -lm �� �߰��ؼ� gcc project.c -lm ���� �������� 
�Ͽ���. 
    How to Compile : gcc project.c -lm

(���߿� filezilla �� c������ �ű��Ŀ� cs20141575.c �� �̸��� �ٲپ���)

5)���� �� ��Ÿ���� �ۼ�
������ gcc cs20141575.c -lm ���� �������� ���Ŀ� ./a.out ���� ���α׷��� �����Ѵ�.
Input the angle : �� ���� �ְ�
Input the number of terms(3~5): �� taylor series�� �����ɶ��� ���� ������ �ִ´�(��3~5�� ����� ���� �����ϸ� error ��� ������ ���.)
 �׷��� ó�������� <Sin, Cos, Tan function using math library> ���������� sin cos tan���� ��¥ sin,cos,tan���̰�
 <Sin, Cos, Tan function using taylor series>������ sin cos tan ���� taylor series �� �̿��� sin cos tan  ���̳��´�
�� �� the error rate of sin,cos,tan ���� taylor series�� �������� math library �ξ��� sin cos tan ���� �������� ��Ÿ���ش�.

After compile by gcc cs20141575.c -lm, execute program by ./a.out
Put angle and num of terms as input, then first values are real sin,cos,tan value and second values are calculated sin,cos,tan value by taylor series
and last values are error rate of math library and taylor series of sin,cos,tan values.