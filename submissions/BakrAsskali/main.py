import pandas as pd

df = pd.read_csv("input.txt", header=None, names=["City", "Product", "Price"])

grouped_df = df.groupby('City')['Price'].sum()

cheapest_city = grouped_df.idxmin()

cheapest_city_data = df[df['City'] == cheapest_city]

sorted_cheapest_products = cheapest_city_data.sort_values(['Price', 'Product']).head(5)

output_file = open('./output.txt', 'w')
output_file.write(f"{cheapest_city} {grouped_df.loc[cheapest_city]:.2f}\n")

for _, row in sorted_cheapest_products.iterrows():
    output_file.write(f"{row['Product']} {row['Price']:.2f}\n")

output_file.close()
