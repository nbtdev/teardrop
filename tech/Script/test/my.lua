function myFunction(a, b)
	
	local f = b
	local r, s
	
	if a then
		print("Hello World: ")
	else
		print("Stinky McFartyPants!")
	end
	
	f = makeTestClass(229, "Cockney")
	r, s = myFunc(f, 12, 4.5, "hi mom", false)
	
	print (r)
	print (s)
	print (globalR)
	print (globalS)
	
	return true
end

function testCall(a, b, c)
	local d = 4
	local e = 5
	d, e = TestCFunction3Args2Rtn(a, b, c)
	print (a)
	print (b)
	print (c)
	print (d)
	print (e)
	
	return d, e
end