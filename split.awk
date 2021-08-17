/>>(.+)/ {
	close (filename)
	filename = $2;
	next
}
{
	print > filename
}
