if  [ $# -ne 2 ]
then
  echo "Usage: compileSO.sh [input] [output]"
  exit 1
fi
g++ -shared -fPIC -std=c++11 -o $2 $1
