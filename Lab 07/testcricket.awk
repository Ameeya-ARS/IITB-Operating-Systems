#!/usr/bin/awk -f
BEGIN {
  FS=":"
  OFS=":"
  maxscore=0
  scoreexceed = 0
  correctmaxscore =0 
  programhighscore = 0

}
{
    if($1=="SCORE"){
        if(($3<=1100 && $3>=0) && ($6<=1100 && $6>=0) ){
            if($3>maxscore){
                maxscore = $3
            }
            if($6>maxscore){
                maxscore = $6
            }           
        }
        else{
            scoreexceed++
        }
    }
    if($1 ~ /Team/){
        programhighscore = $2
        if(maxscore != $2){
            correctmaxscore++
        }
    }
}
END {

    printf("Scores Exceeding 1100 : %d \n", scoreexceed)
    printf("Program Highscore : %d :: Actual Highscore %d \n", programhighscore, maxscore) 


    if(scoreexceed>0){
            printf("Program INCORRECT %d score(s) exceed 1100 \n", scoreexceed)
    }
    
    if(correctmaxscore>0){
            printf("Program is INCORRECT Highscore not correct\n") 
        }

    if(scoreexceed==0 && correctmaxscore==0){
        printf("Program is CORRECT\n");
    }
}