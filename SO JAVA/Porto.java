public class Porto {

    double posizione_x;
    double posizione_y;
    int banchine; 
    int[] lotti;

    public Porto(double x, double y){ 
        posizione_x = x;
        posizione_y = y;
        banchine = Consegna.SO_BANCHINE;
        lotti = new int[Consegna.SO_MERCI];
    }

    public String toString() {
        return "(" + posizione_x + ", " + posizione_y + ")";
    }
}
