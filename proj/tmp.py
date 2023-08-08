import requests

url = 'http://127.0.0.1:8000/api/kgthj4a56etj1tujaetj54at1eahu5e4aj5/sensors/'
j = {'time':'2023', 'temp': 1233.1, 'humid': 1.256562, 'lpg': 13, 'smok': 23.2}
resp = requests.post(url=url, json=j)

# print(resp)
# print(resp.content)


# url = 'http://127.0.0.1:8000/api/kgthj4a56etj1tujaetj54at1eahu5e4aj5/relays/'
# # j = {'time':'2023', 'temp': 3.1, 'humid': 1.256562, 'lpg': 13, 'smok': 23.2}
# j = {"rel1": False, "rel2": True, "rel3": False, "rel4": True}
# resp = requests.post(url=url, json=j)

print(resp)
print(resp.content)