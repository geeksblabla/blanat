import pandas as pd

file_path = "input.txt"
column_names = ['city', 'product', 'price']
df = pd.read_csv(file_path, sep=",", encoding='latin-1',
                 names=column_names, low_memory=False)


sum_prices = df.groupby("city")["price"].sum().sort_values(ascending=False)
sheap_city = sum_prices.index[-1]
sheap_price = sum_prices.values[-1]


# Create dataframe for sheap city :
sheap_city_df = df[df["city"] == sheap_city][["product", "price"]]

# Order dataframe by price and product name :
sheap_city_df = sheap_city_df.sort_values(by=['price', 'product'])

product = []
price = []

# Get the sheap product with price by remove the duplcate product :
for index, row in sheap_city_df[:100].iterrows():

    if row["product"] not in product:
        product.append(row["product"])
        price.append(row["price"])

    if len(product) == 5:
        break

# create the result list :
data_to_save = [f"{sheap_city} {sheap_price}"]

for i in range(0, len(product)):
    data_to_save.append("{} {}".format(product[i], price[i]))

# --------------- #
# save data :
# --------------- #
file_path = 'output.txt'

with open(file_path, 'w') as file:
    for line in data_to_save:
        file.write(line + '\n')
