#ifndef _COLPO_H_

    #define _COLPO_H_

    class colpo
    {
    public:
        colpo();
        ~colpo();


    protected:
        float x,y;
        float speed;
        pegaTextureResource* text_colpo;
        pegaAnimation* anim_colpo;
        float center_x,center_y;
        float radius;


    private:
    };











#endif // _COLPO_H_
