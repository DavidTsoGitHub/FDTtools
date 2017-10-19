pushd
Get-ChildItem -recurse -include MakeFile | %{cd $_.Directory.FullName ; make clean; make;}
popd