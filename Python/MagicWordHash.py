import hashlib

string2hash = '#Zwarte Bodem IPA'
result = hashlib.md5(string2hash.encode())

print(result)
print(result.hexdigest())
print(type(result.hexdigest()))