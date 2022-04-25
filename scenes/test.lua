local a = 0
for i = 0, 100000000 - 1 do
  a = a + i
end
return Vector3f:new(a)