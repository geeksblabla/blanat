import pandas as pd

df = pd.read_csv('input.txt', delimiter=',', names=['city', 'vegetables', 'price'])
city_list = df['city'].tolist()
unic_citys = list(set(city_list))
city_visits = df[df['city'] == unic_citys[0]]['vegetables'].tolist()
sorted_dict = dict(sorted(df.groupby('city')['price'].sum().to_dict().items(), key=lambda x: x[1]))
city_name = next(iter(sorted_dict))
text = f'{city_name}: {sorted_dict[city_name]}\n'
df_1 = df[df['city'].isin([city_name])]
sorted_df = df_1.sort_values(by=['price', 'vegetables'])
top_5 = sorted_df.head(5)
for i in top_5.iterrows():
    text += f"{i[1].vegetables}: {i[1].price}\n"
with open('output.txt', 'w') as file:
    file.write(text)
