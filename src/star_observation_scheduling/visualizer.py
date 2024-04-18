import pandas as pd
import plotly.express as px
import plotly.graph_objects as go
import plotly
from matplotlib import colors


def words(fileobj):
    for line in fileobj:
        for word in line.split():
            yield word


if __name__ == "__main__":

    # Parse command line arguments.
    import argparse
    parser = argparse.ArgumentParser(description='')
    parser.add_argument(
            "certificate",
            type=str,
            help='Certificate file')
    args = parser.parse_args()

    color_list = px.colors.qualitative.Plotly
    with open(args.certificate) as f:
        wordgen = words(f)
        number_of_nights = int(next(wordgen))
        d = []
        for night_id in range(number_of_nights):
            number_of_observations = int(next(wordgen))
            current_time = 0
            for observation_pos in range(number_of_observations):
                observation_id = int(next(wordgen))
                target_id = next(wordgen)
                start_time = int(next(wordgen))
                end_time = int(next(wordgen))
                d.append(dict(
                    night_id=night_id,
                    target_id=target_id,
                    observation_id=observation_id,
                    start_time=start_time,
                    end_time=end_time))

    df = pd.DataFrame(d)

    fig = px.timeline(
            df,
            x_start="start_time",
            x_end="end_time",
            y="night_id",
            color="target_id",
            text="target_id",
            color_discrete_sequence=color_list,
            height=30*number_of_nights)
    fig.update_traces(insidetextanchor='start')

    fig.update_layout(
        yaxis = dict(
            dtick = 1
            )
        )

    fig.update_yaxes(autorange="reversed")
    fig.layout.xaxis.type = 'linear'
    df['delta'] = df['end_time'] - df['start_time']
    for d in fig.data:
        filt = df['target_id'] == d.name
        d.x = df[filt]['delta'].tolist()
    fig.show()
