import java.util.List;

// Compile/run after implementing:
// javac RankingExercises.java && java RankingExercises
public class RankingExercises {
    record Result(String id, double score) {}

    // Exercise 1: return a new list sorted by descending score. Do not mutate input.
    static List<Result> rank(List<Result> input) {
        throw new UnsupportedOperationException("TODO");
    }

    public static void main(String[] args) {
        var input = List.of(new Result("far", 0.3), new Result("near", 0.8));
        assert rank(input).getFirst().id().equals("near");
    }
}
