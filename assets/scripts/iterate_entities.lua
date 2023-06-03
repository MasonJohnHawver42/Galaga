print("iter script")

local view = g_reg:runtime_view(Test)
print(view)
view:each(function(entity)
    trans = g_reg:get(entity, Test)
    print("trans", trans)
end)
