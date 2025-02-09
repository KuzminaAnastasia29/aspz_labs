# Заміна пробілів у рядку на заданий символ

## Опис
Ця програма написана мовою C і виконує заміну всіх пробілів у рядку на вказаний символ. Вона також підраховує кількість виконаних замін.

## Як працює програма
1. Функція `replace` приймає вказівник на рядок (`char *str`) і символ для заміни (`char replacement`).
2. Вона проходить по рядку та змінює кожен пробіл (`' '`) на переданий символ.
3. Повертає кількість замінених пробілів.

## Код програми
```c
#include<stdio.h>

int replace(char *str, char replacement){
	int count = 0;

	if(str == NULL) return 0;

	while(*str){
		if(*str == ' '){
			*str = replacement;
			count++;
		}
		str++;
	}

	return count;
}

int main(){
	char str[] = "The cat sat";
	char replacement = '-';

	int count = replace(str, replacement);

	printf("Modified string: \"%s\"\n", str);
	printf("Number of replacements: %d\n", count);

	return 0;
}
```

## Приклад виконання
### Вхідні дані:
```c
char str[] = "The cat sat";
char replacement = '-';
```

### Вихідні дані:
```
Modified string: "The-cat-sat"
Number of replacements: 2
```

## Компиляція та запуск
1. Відкрийте термінал і перейдіть у директорію з файлом `main.c`.
2. Виконайте компіляцію:
   ```sh
   cc -Wall -o main main.c 
   ```
3. Запустіть програму:
   ```sh
   ./main
   ```

## Автор
Цей код написаний для демонстрації роботи з рядками в мові C.
Кузьміна Анастасія ТВ-33

