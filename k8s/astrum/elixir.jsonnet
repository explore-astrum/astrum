local kube = import '../kube.libsonnet';
{
  service: kube.Service('elixir') {
    metadata: {
      name: 'elixir',
      namespace: 'astrum',
    },
    spec: {
      ports: [
        {
          name: 'socket',
          port: 12000,
          protocol: 'TCP',
          targetPort: 12000,
        },
      ],
      selector: {
        app: 'elixir',
      },
      clusterIP: 'None',
      type: 'ClusterIP',
    },
  },

  deployment: kube.Deployment('elixir') {
    metadata: {
      labels: {
        app: 'elixir',
      },
      namespace: 'astrum',
      name: 'elixir',
    },
    spec+: {
      strategy: {
        type: 'Recreate',
        rollingUpdate: null,
      },
      replicas: 2,
      selector: {
        matchLabels: {
          app: 'elixir',
        },
      },
      template+: {
        metadata: {
          annotations: {
            'ad.datadoghq.com/elixir.logs': '[{"source":"elixir","service":"elixir"}]',
          },
          labels: {
            app: 'elixir',
          },
        },
        spec+: {
          containers_: {
            elixir: kube.Container('elixir') {
              env_: {
                POD_IP: {
                  fieldRef: {
                    fieldPath: 'status.podIP',
                  },
                },
              },
              envFrom: [
                {
                  secretRef: {
                    name: 'elixir',
                    optional: false,
                  },
                },
              ],
              image: 'gcr.io/hifi-atlas/elixir:master',
              imagePullPolicy: 'Always',
              name: 'elixir',
              stdin: true,
              tty: true,
            },
          },
          imagePullSecrets: [
            {
              name: 'dc-zj28p',
            },
          ],
          restartPolicy: 'Always',
        },
      },
    },
  },
}
