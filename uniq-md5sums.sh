for bench in `ls src/`; do
    name=(${bench//./ })
    noop_md5=`md5sum compiled/${name} | awk '{ print $1 }'`
#    echo "$noop_md5 $name"
    for flag in `ls compiled/${name}-*`; do
        flag_md5=`md5sum $flag | awk '{ print $1 }'`
        if [ $flag_md5 == $noop_md5 ]
        then
            `rm "$flag"`
        fi
#        if [ $flag_md5 != $noop_md5 ]
#        then
#            echo "$flag_md5 $flag"
#        fi
    done
done
