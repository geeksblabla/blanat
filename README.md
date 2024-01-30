# Cheapest city #

We are in September 2022, the world has witnessed significant events over the past two years, like  the global COVID-19 pandemic, the Russian invasion of Ukraine and the economic boycott imposed on Russia. These occurrences have had a profound and adverse impact on the international economy, leading to a surge in global inflation reminiscent of that in the 1920s.

As a result, people all over the world are dealing with the rising cost of living, which has hit unprecedented levels. Take Adnan, for example; he's a Moroccan citizen who, like many others, felt the impact. Luckily, Adnan turned Solo-preneur with the help of Djalal two years ago (Thanks Djalal) and now works remotely. This flexibility allows him to live anywhere in Morocco. That's when he got the idea to move to a city where cost of products are more affordable, since his wife isn’t against the idea. Adnan began analyzing a rows of data **(data <= 1,000,000 rows)**  to make a final decision. data contains several Moroccan cities with the price of different products.

Assist Adnan by writing a program, using any allowed language, to find the city with the lowest prices for products. **The challenge is to write an optimized code that can handle large amounts of data and quickly return the sorted results.**

<br>

### SAMPLE INPUT ###
**City name**  **products**  **price**<br>
casa tomato 6<br>
casa potato 4<br>
casa flour 6<br>
casa oil 9<br>
casa sugar5<br>
casa gaz 9<br>
Tetouan tomato 4<br>
Tetouan potato 3<br>
Tetouan flour 5<br>
Tetouan oil 7<br>
Tetouan sugar 4<br>
Tetouan gaz 9<br>
rabat tomato 5<br>
rabat potato 4<br>
rabat flour 5<br>
rabat oil 8<br>
rabat sugar 5<br>
rabat gaz 9<br>

<br>

### SAMPLE OUTPUT ###
**City name**  **products**  **price**<br>
Tetouan potato 3<br>
Tetouan tomato 4<br>
Tetouan sugar 4<br>
Tetouan flour 5<br>
Tetouan oil 7<br>
Tetouan gaz 9<br>
rabat potato 4<br>
rabat tomato 5<br>
rabat flour 5<br>
rabat sugar 5<br>
rabat oil 8<br>
rabat gaz 9<br>
casa potato 4<br>
casa sugar5<br>
casa tomato 6<br>
casa flour 6<br>
casa oil 9<br>
casa gaz 9<br>


**Explanation: Tetouan is the cheapest city because (tetouan) 3+4+4+5+7+9 < (rabat) 4+5+5+5+8+9 < (casa) 4+5+6+6+9+9**
