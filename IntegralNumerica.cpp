#include <iostream>
#include <math.h> // Para utilização do valor "e^x"

using namespace std;

// A variável test define qual a função a ser utilizada, 
// poupando código em funções repetitivas para teste.
// Para os seguintes valores de test:
// test = 0 : e^x dx
// test = 1 : sqrt(1 - x^2) dx
// test = 2 : e^(-x^2) dx

// Variáveis globais para guardar resultado das recursões
float result;
int maxRecurssion;

// Retorna resultado da função definida pela variável test
float Funcao(float x, int test)
{
	switch (test)
	{
	case 0:
		return exp(x);
		break;
	case 1:
		return sqrt(1 - pow(x, 2));
		break;
	case 2:
		return exp(-pow(x, 2));
		break;
	default:
		return 0;
		break;
	}
	
}

// Retorna resultado da derivada 2 definida pela variável test
float Derivada2(float x, int test)
{
	switch (test)
	{
	case 0:
		return exp(x);
		break;
	case 1:
		return 1 / pow((1 - pow(x, 2)), 3 / 2);
		break;
	case 2:
		return -2 * (exp(-pow(x, 2)) * x);
		break;
	default:
		return 0;
		break;
	}
}

// Retorna resultado da derivada 4 definida pela variável test
float Derivada4(float x, int test)
{
	switch (test)
	{
	case 0:
		return exp(x);
		break;
	case 1:
		return (12 * pow(x, 2)) + 3 / pow((1 - pow(x, 2)), 7 / 2);
		break;
	case 2:
		return 4 * exp(-pow(x, 2)) * (4 * pow(x, 4) - 12 * pow(x, 2) + 3);
		break;
	default:
		return 0;
		break;
	}
}

// Retorna o resultado pelo método Retangular
float rectangular(float a, float b, int test)
{
	return (b - a) * Funcao((a + b) / 2, test);	
}

// Retorna o erro do método Retangular
float rectangularError(float a, float b, int test)
{
	return -((pow((b - a), 3) / 24) * Derivada2(a + ((b - a) / 2), test));
}

// Retorna o resultado pelo método Trapézio
float trapezium(float a, float b, int test)
{
	return (b - a) * ((Funcao(a, test) + Funcao(b, test)) / 2);
}

// Retorna o erro do método Trapézio
float trapeziumError(float a, float b, int test)
{
	return -((pow((b - a), 3) / 12) * Derivada2(a + ((b - a) / 2), test));
}

// Retorna o resultado do método Simpson
float simpson(float a, float b, int test)
{
	return (b - a) * ((Funcao(a, test) + (4 * Funcao((a + b) / 2, test)) + Funcao(b, test)) / 6);
}

// Retorna o erro do método Simpson
float simpsonError(float a, float b, int test)
{
	return -((pow((b - a), 5) / 2880) * Derivada4(a + ((b - a) / 2), test));
}

// Cálculo de quadratura adaptativa para o método Retangular
float rectangularAdapt(float a, float b, float maxError, int test, int recurssion)
{
	float err = abs(rectangularError(a, b, test));

	// Zera o valor inicial se for a primeira recurssão
	if (recurssion == 0)
	{
		result = 0;
	}

	// Retorna erro = 1000 caso atinja um limite de recurssão
	if (recurssion > maxRecurssion)
	{
		return 1000;
	}

	// Verifica se o erro está dentro do exigido ou se inicia a recurssão
	if ( err > maxError)
	{
		float err1 = rectangularAdapt(a, (b - a)/2 + a, maxError, test, recurssion++);
		float err2 = rectangularAdapt((b - a) / 2 + a, b, maxError, test, recurssion++);

		err = err1 + err2;
	}
	else
	{
		result += rectangular(a, b, test);
	}
	
	return err;
}

// Cálculo de quadratura adaptativa para o método Trapézio
float trapeziumAdapt(float a, float b, float maxError, int test, int recurssion)
{
	float err = abs(trapeziumError(a, b, test));

	// Zera o valor inicial se for a primeira recurssão
	if (recurssion == 0)
	{
		result = 0;
	}

	// Retorna erro = 1000 caso atinja um limite de recurssão
	if (recurssion > maxRecurssion)
	{
		return 1000;
	}

	// Verifica se o erro está dentro do exigido ou se inicia a recurssão
	if (err > maxError)
	{
		float err1 = trapeziumAdapt(a, (b - a) / 2 + a, maxError, test, recurssion++);
		float err2 = trapeziumAdapt((b - a) / 2 + a, b, maxError, test, recurssion++);

		err = err1 + err2;
	}
	else
	{
		result += trapezium(a, b, test);
	}

	return err;
}

// Cálculo de quadratura adaptativa para o método de Simpson
float simpsonAdapt(float a, float b, float maxError, int test, int recurssion)
{
	float err = abs(simpsonError(a, b, test));

	// Zera o valor inicial se for a primeira recurssão
	if (recurssion == 0)
	{
		result = 0;
	}

	// Retorna erro = 1000 caso atinja um limite de recurssão
	if (recurssion > maxRecurssion)
	{
		return 1000;
	}

	// Verifica se o erro está dentro do exigido ou se inicia a recurssão
	if (err > maxError)
	{
		float err1 = simpsonAdapt(a, (b - a) / 2 + a, maxError, test, recurssion++);
		float err2 = simpsonAdapt((b - a) / 2 + a, b, maxError, test, recurssion++);

		err = err1 + err2;
	}
	else
	{
		result += simpson(a, b, test);
	}

	return err;
}


int main()
{
	// Teste das funções com o intervalo [0,1]
	cout << "Testing functions on step [0,1]:" << endl << endl;

	float a = 0;
	float b = 1;

	for (int i = 0; i < 3; i++)
	{
		float metodo1 = rectangular(a, b, i);
		float metodo2 = trapezium(a, b, i);
		float metodo3 = simpson(a, b, i);

		float erro1 = rectangularError(a, b, i);
		float erro2 = trapeziumError(a, b, i);
		float erro3 = simpsonError(a, b, i);

		switch (i)
		{
		case 0:
			cout << "Function: e^x dx" << endl << endl;
			break;
		case 1:
			cout << "Function: sqrt(1 - x^2) dx" << endl << endl;
			break;
		case 2:
			cout << "Function: e^(-x^2) dx" << endl << endl;
			break;
		default:
			break;
		}

		cout << "Rectangular result: " << metodo1 << " | error: " << erro1 << endl;
		cout << "Trapezium result:   " << metodo2 << " | error: " << erro2 << endl;
		cout << "Simpson result:     " << metodo3 << " | error: " << erro3 << endl << endl;
	}
	cin.get();

	// Teste de quadratura adaptativa
	cout << "Testing functions on step [0,1] with recurssion for error = 0.0000001:" << endl << endl;
	float calcError;
	maxRecurssion = 100;

	for (int i = 0; i < 3; i++)
	{
		switch (i)
		{
		case 0:
			cout << "Function: e^x dx" << endl << endl;
			break;
		case 1:
			cout << "Function: sqrt(1 - x^2) dx" << endl << endl;
			break;
		case 2:
			cout << "Function: e^(-x^2) dx" << endl << endl;
			break;
		default:
			break;
		}

		calcError = rectangularAdapt(a, b, 0.0000001, i, 0);
		cout << "Rectangular result: " << result << " | error: " << calcError << endl;
		calcError = trapeziumAdapt(a, b, 0.0000001, i, 0);
		cout << "Trapezium result:   " << result << " | error: " << calcError << endl;
		calcError = simpsonAdapt(a, b, 0.0000001, i, 0);
		cout << "Simpson result:     " << result << " | error: " << calcError << endl << endl;
	}
	cin.get();
}