

print("test script")

entity2 = g_reg:create()
g_reg:emplace(entity2, Test(69, 20))
print(g_reg, entity2, Test.type_id())
print(g_reg:size())