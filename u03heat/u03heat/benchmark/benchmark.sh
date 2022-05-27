echo "New Benchmark \n" > results.txt
for f in *; do
    if [ -d "$f" ]; then
        cd $f
        echo $f
        echo "[ $f ]" >> ../results.txt
        echo CMakeLists.txt
        cd build
        cmake ..
        make
        ./heat >> ../../results.txt
        echo "" >> ../../results.txt
        # rm * -rf
        cd ../..

    fi
done
