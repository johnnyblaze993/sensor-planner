package com.example.sensorplanner.api;

import com.example.sensorplanner.model.PlanResult;
import com.example.sensorplanner.model.RankedTarget;
import com.example.sensorplanner.service.PlannerService;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.web.servlet.WebMvcTest;
import org.springframework.test.context.bean.override.mockito.MockitoBean;
import org.springframework.test.web.servlet.MockMvc;
import java.util.List;

import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.when;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.get;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.post;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.jsonPath;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;

@WebMvcTest(PlanController.class)
class PlanControllerTest {
    @Autowired private MockMvc mockMvc;
    @MockitoBean private PlannerService plannerService;

    @Test
    void returnsAPlanForAValidRequest() throws Exception {
        when(plannerService.generatePlan(any())).thenReturn(
                new PlanResult(List.of(new RankedTarget("target-1", 15.8, 0.86, true))));
        mockMvc.perform(post("/api/plans").contentType("application/json").content(validRequest()))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.rankedTargets[0].id").value("target-1"));
    }

    @Test
    void rejectsAnInvalidLatitudeBeforeCallingCpp() throws Exception {
        mockMvc.perform(post("/api/plans").contentType("application/json")
                        .content(validRequest().replace("34.725", "134.725")))
                .andExpect(status().isBadRequest())
                .andExpect(jsonPath("$.message").value("Request validation failed"));
    }

    @Test
    void rejectsAnEmptyTargetList() throws Exception {
        mockMvc.perform(post("/api/plans").contentType("application/json")
                        .content("""
                                {"sensor":{"latitude":34.725,"longitude":-86.65,"altitudeMeters":180},
                                 "targets":[]}
                                """))
                .andExpect(status().isBadRequest())
                .andExpect(jsonPath("$.fields.targets").exists());
    }

    @Test
    void reportsHealth() throws Exception {
        mockMvc.perform(get("/api/health"))
                .andExpect(status().isOk()).andExpect(jsonPath("$.status").value("UP"));
    }

    private String validRequest() {
        return """
                {"sensor":{"latitude":34.725,"longitude":-86.65,"altitudeMeters":180},
                 "targets":[{"id":"target-1","latitude":34.8,"longitude":-86.5,"altitudeMeters":220}]}
                """;
    }
}
