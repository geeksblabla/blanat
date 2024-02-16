# Blanat Challenge

Blanat Challenge is a coding challenge designed to test your coding skills and your ability to write optimized code. [The challenge](#challenge-cheapest-city) primarily focuses on writing a program capable of efficiently handling large amounts of data and promptly returning the solution. It is open to everyone, and you can submit your code in any of the following languages: C, C++, Java, Rust, PHP or JavaScript. This is your chance to demonstrate that your favorite programming language is the best and the fastest one! 😀

> This challenge is part of BlaBlaConf 2024, Make sure to check the [event](https://blablaconf.com)

### Contents

- [Contents](#contents)
- [Submit your solution](#submit-your-solution)
- [Challenge: Cheapest city](#challenge-cheapest-city)
  - [Constraints](#constraints)
  - [Output format](#output-format)
  - [Rules](#rules)
- [I have a question](#i-have-a-question)
- [Leaderboard](#leaderboard)
- [Prize](#prize)

## Submit your solution

- 1. Fork this repository
- 2. Create a folder under /submissions folder with your Github handle as name: `chermehdi` for example.
- 3. Read the challenge [carefully](#challenge-cheapest-city), rules and constraints and start coding with your favorite language. Java, C, C++, Rust, or JavaScript.
- 4. Optimize your code and make sure that it is working as expected.
- 5. Open a PR with the title `Challenge: Username submission` and wait for the review. and hopefully, your PR will be merged.
- 6. Our team will run your code with a large dataset(1 billion rows) and add your name to the leaderboard, along with the time it took to run your code.

## Challenge: Cheapest city

We are in September 2022, the world has witnessed significant events over the past two years, like the global COVID-19 pandemic, the Russian invasion of Ukraine and the economic boycott imposed on Russia. These occurrences have had a profound and adverse impact on the international economy, leading to a surge in global inflation reminiscent of that in the 1920s.

As a result, people all over the world are dealing with the rising cost of living, which has hit unprecedented levels. Take Adnan, for example; he's a Moroccan citizen and, like many others, felt the impact. Luckily, Adnan turned Solo-preneur with the help of Djalal two years ago (Thanks Djalal) and now works remotely. This flexibility allows him to live anywhere in Morocco. That's when he got the idea to move to a city where the cost of products is more affordable since his wife isn’t against the idea 😁. Adnan starts analyzing rows of data **(data <= 1,000,000,000 rows)** to make a final decision. The data contains several Moroccan cities with the prices of different products.

Assist Adnan by writing a program, using any allowed language, to find the city with the lowest prices for products. **The challenge is to write an optimized code that can handle large amounts of data and quickly return the sorted results.**

<br>

### Constraints:

- 1.00 <= price <= 10.00
- we have 101 cities.
- We have 100 products.
- Each city may have the same product duplicated several times with different or identical prices.
- Your code will process a 10^9 (1 billion) line CSV file.
- You are allowed to code with any language from this list: C, C++, Java, Python, Rust, JavaScript, PHP.

### Output format

- Output has 6 lines.
- The first line contains the name of the cheapest city and the total price of products in that city, separated by one space.
- The remaining 5 lines display the first 5 cheapest products sorted by price in ascending order. In the case of two products with the same price, they are sorted alphabetically. For example, "Potato" and "Tomato" in the output below
- Don't print the same product twice. If you have multiple instances of the same product in the top 5 cheapest products, only print the cheapest one.
- Like the 1st line, other lines have one space between the product and the price.
- All the prices are decimal and they have a scale of 2.
- Be careful when you format the string that you will print, the spaces have to be respected, you will receive a **Wrong answer** even if the data is correct.
- Each line is trimmed (no spaces at the beginning or the end of each line).

### Rules

- Make sure that your code is compiling. 😄 It may seem trivial, but sometimes we receive code that doesn't compile.
- If your solution is correct, we will count it and add your name to the leaderboard, if not we will discard it.
- When you feel that your code is ready, create a folder under the "/submissions" folder with your GitHub handle as the name. Add the code to this folder and open a pull request (PR).
- Before creating the PR, your code has to produce the same result as the output below.
- You are allowed to submit the code several times with different langs.
- Each code will be reviewed manually. Any fraudulent attempt will result in your account being banned, and you will be disqualified from the challenge.
- Java solutions should use `Main.java` and have a `public class Main` as a top-level class.
- Contestants are supposed to read input from the file in their current working directory named `input.txt` and write the output to a file in the same directory named `output.txt`.

<br>

### INPUT

```csv
city,products,price
casa,tomato,6.23
casa,tomato,7.23
casa,tomato,8.23
casa,tomato,9.23
casa,potato,4.21
casa,flour,6.24
casa,oil,9.24
casa,oil,9.94
casa,oil,8.24
casa,sugar,5.23
casa,sugar,5.23
casa,sugar,5.23
casa,gaz,9.25
Tetouan,tomato,4.5
Tetouan,tomato,3.5
Tetouan,potato,3.5
Tetouan,flour,5.2
Tetouan,oil,7.7
Tetouan,oil,6.7
Tetouan,sugar,4.5
Tetouan,gaz,9.3
Tetouan,gaz,2.3
Tetouan,gaz,1.3
rabat,tomato,5.85
rabat,tomato,9.85
rabat,potato,4.65
rabat,flour,5.65
rabat,oil,8.15
rabat,oil,9.15
rabat,oil,7.15
rabat,sugar,9.05
rabat,sugar,9.15
rabat,gaz,9.45
```

<br>

### OUTPUT

```txt
Tetouan 48.50
gaz 1.30
potato 3.50
tomato 3.50
flour 5.20
oil 6.70
```

**Explanation: Tetouan is the cheapest city because it has a total price of 48.50, while Casa has a total price of 93.73 and Rabat has a total price of 78.10.**

## I have a question

If you have any questions, feel free to add them to the [Discussions](https://github.com/geeksblabla/blanat/discussions/categories/q-a), and our team and contributors will be happy to help you.

## Leaderboard

The primary criterion for leaderboard ranking is execution time. The fastest code will be ranked at the top.

> Coming soon

## Prize

The prize will be announced on the opening day of BlaBlaConf 2024. Stay tuned for more information. Make sure to check the live event on [YouTube](https://www.youtube.com/watch?v=-peiFexaSWg).
