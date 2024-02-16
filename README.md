# Blanat Challenge

Blanat Challenge is a coding challenge designed to test your coding skills and your ability to write optimized code. [The challenge](#challenge-cheapest-city) primarily focuses on writing a program capable of efficiently handling large amounts of data and promptly returning the solution. It is open to everyone, and you can submit your code in any of the following languages: C, C++, Java, Rust, or JavaScript. This is your chance to demonstrate that your favorite programming language is the best and the fastest one! 😀

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
- 3. Read the challenge [carefully](#challenge-cheapest-city) and start coding with your favorite language. Java, C, C++, Rust, or JavaScript.
- 4. Optimize your code and make sure that it is working as expected.
- 5. Open a PR with the title `Challenge: Username submission` and wait for the review. and hopefully, your PR will be merged.
- 6. Our team will run your code with a large dataset(1 billion rows) and add your name to the leaderboard, along with the time it took to run your code.

## Challenge: Cheapest city

We are in September 2022, the world has witnessed significant events over the past two years, like the global COVID-19 pandemic, the Russian invasion of Ukraine and the economic boycott imposed on Russia. These occurrences have had a profound and adverse impact on the international economy, leading to a surge in global inflation reminiscent of that in the 1920s.

As a result, people all over the world are dealing with the rising cost of living, which has hit unprecedented levels. Take Adnan, for example; he's a Moroccan citizen, like many others, felt the impact. Luckily, Adnan turned Solo-preneur with the help of Djalal two years ago (Thanks Djalal) and now works remotely. This flexibility allows him to live anywhere in Morocco. That's when he got the idea to move to a city where cost of products are more affordable, since his wife isn’t against the idea. Adnan began analyzing a rows of data **(data <= 1,000,000 rows)** to make a final decision. data contains several Moroccan cities with the price of different products.

Assist Adnan by writing a program, using any allowed language, to find the city with the lowest prices for products. **The challenge is to write an optimized code that can handle large amounts of data and quickly return the sorted results.**

<br>

### Constraints:

- 1.00 <= price <= 10.00
- we have 101 cities.
- We have 100 products.
- Each city may have the same product duplicated several times with different or identical prices.
- Your code will process a 10^9 (1 billion) line CSV file.
- you are allowed to code with any lang from this list (c, c++, java, python, rust, JS, PHP).

### Output format

- output has 6 lines.
  - 1st line has the name of cheapest city and the total price of products in that city separated with one space.
  - The remaining 5 lines display the first 5 cheapest products sorted by price in ascending order. In case of two products with the same price, they are sorted alphabetically. For example, "Potato" and "Tomato" in the output below
  - Dont print the same product twice, in case you have the same product more than one time in the top 5 cheapest products just print the the cheapest one.
  - Like the 1st line, other lines have one space between the product and the price.
- All the prices are decimal and they have a scale of 2.
- Be careful when you format the string that you will print, the spaces have to be respectd, you will receive a **Wrong answer** even if the data is correct.
- Each line is trimed (no spaces at the beginnig or the end of each line).

### Rules

- make sure that your code is compiling :D (It looks stupid but it happpens that we recieve code doesnt compile).
- when you fell that your code is ready, create a folder under /submissions folder with your github handle as name, add the code to this folder and open a PR.
- Before create the PR, your code has to give you the same result of the output below.
- you are allowed to submit the code several times with different langs.
- Each code will be reviewed manually, any fraude attempt, your account will be banned and you will be disqualified from the challenge.
- you will receive ACCEPTED or WRONG ANSWER as answers.

<br>

### INPUT

**City_name**,**products**,**price**<br>
casa,tomato,6.23<br>
casa,tomato,7.23<br>
casa,tomato,8.23<br>
casa,tomato,9.23<br>
casa,potato,4.21<br>
casa,flour,6.24<br>
casa,oil,9.24<br>
casa,oil,9.94<br>
casa,oil,8.24<br>
casa,sugar,5.23<br>
casa,sugar,5.23<br>
casa,sugar,5.23<br>
casa,gaz,9.25<br>
Tetouan,tomato,4.5<br>
Tetouan,tomato,3.5<br>
Tetouan,potato,3.5<br>
Tetouan,flour,5.2<br>
Tetouan,oil,7.7<br>
Tetouan,oil,6.7<br>
Tetouan,sugar,4.5<br>
Tetouan,gaz,9.3<br>
Tetouan,gaz,2.3<br>
Tetouan,gaz,1.3<br>
rabat,tomato,5.85<br>
rabat,tomato,9.85<br>
rabat,potato,4.65<br>
rabat,flour,5.65<br>
rabat,oil,8.15<br>
rabat,oil,9.15<br>
rabat,oil,7.15<br>
rabat,sugar,9.05<br>
rabat,sugar,9.15<br>
rabat,gaz,9.45<br>

<br>

### OUTPUT

**City_name**,**products**,**price**<br>
Tetouan 48.50<br>
gaz 1.30<br>
potato 3.50<br>
tomato 3.50<br>
flour 5.20<br>
oil 6.70<br>

**Explanation: Tetouan is the cheapest city because it has total price of 48.50 while casa has total price of 93.73 and rabat 78.10**

## I have a question

If you have any questions, feel free to add them to the [Discussions](https://github.com/geeksblabla/blanat/discussions/categories/q-a), and our team and contributors will be happy to help you.

## Leaderboard

Coming soon

## Prize

The prize will be announced on the opening day of BlaBlaConf 2024. Stay tuned for more information. Make sure to check the live event on [YouTube](https://www.youtube.com/watch?v=-peiFexaSWg).
