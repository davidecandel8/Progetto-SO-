public class Nave {

    double posizione_x;
    double posizione_y;
    int[] porti_carichi; 
    int[] porti_scarichi; 
    int priorità; 

    public Nave(double x, double y) {
        posizione_x = x;
        posizione_y = y;
    }

    public String toString() {
        return "(" + posizione_x + ", " + posizione_y + ")";
    }
}
